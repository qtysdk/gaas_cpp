#pragma once

#include <string>
#include <map>
#include "../model/Game.h"

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>

class IGameRepository {
public:
    virtual std::shared_ptr<Game> create(std::string playerName) = 0;

    virtual std::shared_ptr<Game> findGameById(std::string gameId) = 0;

    virtual void save(std::shared_ptr<Game> game) = 0;
};

std::shared_ptr<IGameRepository> getGameRepo();


class InMemoryGameRepository : public IGameRepository {

private:
    std::map<std::string, std::shared_ptr<Game>> storage;

public:
    ~InMemoryGameRepository();

    std::shared_ptr<Game> create(std::string playerName);

    std::shared_ptr<Game> findGameById(std::string gameId);

    void save(std::shared_ptr<Game> game);
};


using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

class MongoDBGameRepository : public IGameRepository {
private:
    std::shared_ptr<mongocxx::client> client;
    mongocxx::collection col;

public:
    MongoDBGameRepository();

    std::shared_ptr<Game> create(std::string playerName) override;

    std::shared_ptr<Game> findGameById(std::string gameId) override;

    void save(std::shared_ptr<Game> game) override;


};
