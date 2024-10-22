#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#define REQUEST_SPOT "request_spot"
#define ASSIGN_SPOT "assign_spot"
#define CHECKOUT "checkout"
#define PASS_TIME "pass_time"
#define NORMAL "normal"
#define COVERED "covered"
#define CCTV "CCTV"
#define STATIC_PRICE_COVERED 50
#define PER_DAY_COVERED 30
#define STATIC_PRICE_CCTV 80
#define PER_DAY_CCTV 60
#define OCCUPIED "occupied"
#define FREE "free"

using namespace std;

struct spot_property
{
    string type;
    int size;
    string status;
    int time;
};

void availableSpot(string car_name, map<string, int> &cars, map<int, spot_property> &parking_spots, map<int, pair<int, int>> &prices)
{
    for (auto it = parking_spots.begin(); it != parking_spots.end(); it++)
    {
        if (it->second.size == cars[car_name])
        {
            if (it->second.status != OCCUPIED)
            {
            cout << it->first << ": " << it->second.type << " ";
            if (it->second.type == NORMAL)
                cout << prices[cars[car_name]].first << " " << prices[cars[car_name]].second << endl;
            else if (it->second.type == COVERED)
            {
                int static_price = prices[cars[car_name]].first + STATIC_PRICE_COVERED;
                int per_day_price = prices[cars[car_name]].second + PER_DAY_COVERED;
                cout << static_price << " " << per_day_price << endl;
            }
            else if (it->second.type == CCTV)
            {
                int static_price = prices[cars[car_name]].first + STATIC_PRICE_CCTV;
                int per_day_price = prices[cars[car_name]].second + PER_DAY_CCTV;
                cout << static_price << " " << per_day_price << endl;
            }
            }
        }
    }
}

void assignSpot(int id, map<int, spot_property> &parking_spots)
{
    if (parking_spots[id].status == FREE)
    {
        parking_spots[id].status = OCCUPIED;
        cout << "Spot " << id << " is occupied now." << endl;
    }
}

void passTime(int time, map<int, spot_property> &parking_spots)
{
    map<int, spot_property>::iterator it;
    for (it = parking_spots.begin(); it != parking_spots.end(); it++)
    {
        if (it->second.status == OCCUPIED)
        {
            it->second.time = it->second.time + time;
        }
    }
}

void checkout(int id, map<int, spot_property> &parking_spots, map<int, pair<int, int>> &prices)
{
    if (parking_spots[id].status == OCCUPIED)
    {
        parking_spots[id].status = FREE;
        parking_spots[id].time = 0;
        cout << "Spot " << id << " is free now." << endl;
        int cost{0};
        if (parking_spots[id].type == NORMAL)
        {
            cost = prices[parking_spots[id].size].first + (parking_spots[id].time) * prices[parking_spots[id].size].second;
        }
        else if (parking_spots[id].type == COVERED)
        {
            cost = prices[parking_spots[id].size].first + STATIC_PRICE_COVERED + (parking_spots[id].time) * (prices[parking_spots[id].size].second + PER_DAY_COVERED);
        }
        else if (parking_spots[id].type == CCTV)
        {
            cost = prices[parking_spots[id].size].first + STATIC_PRICE_CCTV + (parking_spots[id].time) * (prices[parking_spots[id].size].second + PER_DAY_CCTV);
        }
        cout << "Total cost: " << cost << endl;
    }
}

void run(map<string, int> &cars, map<int, spot_property> &parking_spots, map<int, pair<int, int>> &prices)
{
    string line;
    stringstream line_parse;
    while (getline(cin, line))
    {
        line_parse = stringstream();
        line_parse << line;
        line_parse >> line;
        if (line == REQUEST_SPOT)
        {
            line_parse >> line;
            availableSpot(line, cars, parking_spots, prices);
        }
        else if (line == ASSIGN_SPOT)
        {
            line_parse >> line;
            assignSpot(stoi(line), parking_spots);
        }
        else if (line == CHECKOUT)
        {
            line_parse >> line;
            checkout(stoi(line), parking_spots, prices);
        }
        else if (line == PASS_TIME)
        {
            line_parse >> line;
            passTime(stoi(line), parking_spots);
        }
    }
}

void carFile(string file_name, map<string, int> &cars)
{
    fstream car_file(file_name);
    string line;
    while (car_file)
    {
        getline(car_file, line);
        size_t pos = line.find(',');
        cars[line.substr(0, pos)] = stoi(line.substr(pos + 1));
    }
}

void parkingSpots(string file_name, map<int, spot_property> &parking_spots)
{
    fstream spots_file(file_name);
    string line;
    int id;
    spot_property type_size;
    while (getline(spots_file, line))
    {
        size_t pos = line.find(',');
        id = stoi(line.substr(0, pos));
        line = line.substr(pos + 1);
        pos = line.find(',');
        type_size.size = stoi(line.substr(0, pos));
        type_size.type = line.substr(pos + 1);
        type_size.status = FREE;
        type_size.time = 0;
        parking_spots[id] = type_size;
    }
}

void priceFile(string file_name, map<int, pair<int, int>> &prices)
{
    fstream price_file(file_name);
    string line;
    int size;
    int price;
    size_t pos;
    while (getline(price_file, line))
    {
        pos = line.find(',');
        size = stoi(line.substr(0, pos));
        line = line.substr(pos + 1);
        pos = line.find(',');
        prices[size] = {stoi(line.substr(0, pos)), stoi(line.substr(pos + 1))};
    }
}

void initialize(char *argv[], map<string, int> &cars, map<int, spot_property> &parking_spots, map<int, pair<int, int>> &prices)
{
    carFile(argv[1], cars);
    parkingSpots(argv[2], parking_spots);
    priceFile(argv[3], prices);
}

int main(int argc, char *argv[])
{
    map<string, int> cars;
    map<int, spot_property> parking_spots;
    map<int, pair<int, int>> prices;
    initialize(argv, cars, parking_spots, prices);
    run(cars, parking_spots, prices);
}
