#include <random>
#include <iostream>
#include <thread>
#include "GameRepository.h"

static std::shared_ptr<IGameRepository> _repo;

std::shared_ptr<IGameRepository> getGameRepo() {
    if (_repo == nullptr) {
        _repo = std::make_shared<InMemoryGameRepository>();
    }
    return _repo;
}

InMemoryGameRepository::~InMemoryGameRepository() {
    storage.clear();
}


const std::string randomId() {
    srand(time(0));
    return std::to_string(rand());
}

std::shared_ptr<Game> InMemoryGameRepository::create(std::string playerName) {
    std::shared_ptr<Game> game = std::make_shared<Game>();
    game->id = randomId();
    game->playerName = playerName;
    storage[game->id] = game;
    return game;
}

std::shared_ptr<Game> InMemoryGameRepository::findGameById(std::string gameId) {
    return storage[gameId];
}

void InMemoryGameRepository::save(std::shared_ptr<Game> game) {
    // do nothing, there are all in the memory
}

// MongoDriver must be initialized exactly once
mongocxx::instance instance{};

MongoDBGameRepository::MongoDBGameRepository() {

    char *mongoUri = std::getenv("GAAS_MONGODB_URI");

    if (!mongoUri) {
        throw std::runtime_error("Please configure env[GAAS_MONGODB_URI]");
    }

    std::cout << "URI: " << mongoUri << std::endl;
    client = std::make_shared<mongocxx::client>(mongocxx::uri{std::string(mongoUri)});

    // database: test
    // collection: test
    col = (*client)["test"]["test"];

//    std::chrono::seconds sleep_duration(15);
//    std::this_thread::sleep_for(sleep_duration);
}


std::shared_ptr<Game> MongoDBGameRepository::create(std::string playerName) {
    // Example from: https://mongocxx.org/mongocxx-v3/tutorial/#insert-one-document

    // TODO extract connection uri to outside
    std::shared_ptr<Game> game = std::make_shared<Game>();

    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
            << "player_name" << playerName
            << "won" << false
            << "answer" << game->answer
            << "history" << bsoncxx::builder::stream::open_array
            << close_array
            << bsoncxx::builder::stream::finalize;

    game->playerName = playerName;
    game->won = false;

    try {
        auto view = doc_value.view();
        auto result = col.insert_one(view);
        // code that might throw an "expected element type k_string" exception

        // Print the ID of the inserted document
        if (result) {
            game->id = result->inserted_id().get_oid().value.to_string();
        } else {
            std::cout << "Insert failed" << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << "Caught bsoncxx exception: " << e.what() << std::endl;
    }


    return game;
}

std::shared_ptr<Game> MongoDBGameRepository::findGameById(std::string gameId) {
    std::shared_ptr<Game> game = std::make_shared<Game>();

    // Construct a query for the document with the desired ObjectId
    auto builder = bsoncxx::builder::stream::document{};
    auto query = builder
            << "_id" << bsoncxx::oid{gameId}
            << bsoncxx::builder::stream::finalize;

    // Find the document matching the query
    auto result = col.find_one(query.view());

    if (result) {
        // Extract the relevant fields from the document view
        auto view = result->view();
        auto id = view["_id"].get_oid().value.to_string();
        auto playerName = view["player_name"].get_string().value.to_string();
        auto answer = view["answer"].get_int32().value;
        auto won = view["won"].get_bool().value;
        auto history_array = view["history"].get_array().value;
        std::list<std::shared_ptr<Record>> history;
        for (auto it = history_array.cbegin(); it != history_array.cend(); ++it) {
            auto record = std::make_shared<Record>();
            record->guess = (*it)["guess"].get_int32().value;
            record->respond = (*it)["respond"].get_string().value.to_string();
            history.push_back(record);
        }

        // Construct a Game object from the extracted fields
        auto game = std::make_shared<Game>();
        game->id = id;
        game->playerName = playerName;
        game->answer = answer;
        game->won = won;
        game->history = history;

        return game;
    }

    std::cout << "Document not found" << std::endl;
    return nullptr;
}


void MongoDBGameRepository::save(std::shared_ptr<Game> game) {
    // Construct a query that matches the document with the desired ObjectId
    auto query = bsoncxx::builder::stream::document{} << "_id" << bsoncxx::oid{game->id}
                                                      << bsoncxx::builder::stream::finalize;


    // Create an array for the history records
    auto history_array = bsoncxx::builder::stream::array{};
    for (const auto &record: game->history) {
        auto record_doc = bsoncxx::builder::stream::document{};
        record_doc << "guess" << record->guess << "respond" << record->respond;
        history_array << record_doc;
    }

    // Construct a document that represents the updated fields of the document
    auto builder = bsoncxx::builder::stream::document{};
    auto update = builder
            << "$set" << bsoncxx::builder::stream::open_document
            << "player_name" << game->playerName
            << "answer" << game->answer
            << "won" << game->won
            << "history" << history_array
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize;

    // Update the document matching the query with the updated fields
    col.update_one(query.view(), update.view());
}