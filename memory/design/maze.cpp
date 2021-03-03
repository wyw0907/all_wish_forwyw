/* ****************************
 * 设计模式一书中讲解creator模式的迷宫算法设计
 * author wuyunwei
 * date   2020 01 01
 * 
 * ****************************/

#include <exception>
#include <map>
enum Direction {
    North,
    South,
    East,
    West,
};

class MapSite
{
public:
    virtual void Enter() = 0;
};

class Room : public MapSite
{
public:
    Room(int no);
    
    MapSite *GetSide(Direction d) const;

    void SetSide(Direction d, MapSite *);

    virtual void Enter() override;

    int  Number() const ;
private:
    MapSite *   _sides[4];
    int         _roomNumber;
};

Room::Room(int no) : _roomNumber(no)
{

}
    
//补上定义
MapSite *Room::GetSide(Direction d) const
{
    return _sides[d];
}
//补上定义
void Room::SetSide(Direction d, MapSite *s)
{
    _sides[d] = s;
}
//补上定义
int Room::Number( ) const 
{
    return _roomNumber;
}

//补上定义
void Room::Enter()
{
    //
}

class Wall : public MapSite
{
public:
    Wall();

    virtual void Enter();

};

//补上定义
void Wall::Enter()
{
    //
}

class Door : public MapSite
{
public:
    Door(Room * = 0, Room * = 0);

    virtual void Enter() override;

    Room *OtherSideFrom(Room *r);

private:
    Room *_room1;
    Room *_room2;
    bool _isOpen;
};

//补上定义
Door::Door(Room * r1, Room * r2) : _room1(r1), _room2(r2)
{
    //
}

//补上定义
void Door::Enter()
{
    //
}
//补上定义
Room *Door::OtherSideFrom(Room *r)
{
    if (r == _room1)
    {
        return _room2;
    }
    else if (r == _room2)
    {
        return _room1;
    }
    else
    {
        throw std::logic_error("error Room");
    }
    
}

class Maze
{
public:
    Maze();
    void AddRoom(Room *);
    Room *RoomNo(int) const;

private:
    std::map<int, Room *> _rooms;
};

Maze::Maze()
{

}

void Maze::AddRoom(Room *r)
{
    _rooms[r->Number()] = r;
}

Room *Maze::RoomNo(int no) const
{
    auto it = _rooms.find(no);
    return it->second;
}


inline Maze * MazeGame()
{
    Maze *aMaze = new Maze;
    Room *r1 = new Room(1);
    Room *r2 = new Room(2);
    Door *theDoor = new Door(r1, r2);

    aMaze->AddRoom(r1);
    aMaze->AddRoom(r2);

    r1->SetSide(North, new Wall);
    r1->SetSide(East, theDoor);
    r1->SetSide(South, new Wall);
    r1->SetSide(West, new Wall);

    r2->SetSide(North, new Wall);
    r2->SetSide(East, new Wall);
    r2->SetSide(South, new Wall);
    r2->SetSide(West, theDoor);

    return aMaze;
}

// Abstract Factory 抽象工厂

namespace {

class MazeFactory
{
public:
    MazeFactory();

    virtual Maze *MakeMaze() const
    {
        return new Maze;
    }

    virtual Wall *MakeWall() const
    {
        return new Wall;
    }

    virtual Room *MakeRoom(int n) const
    {
        return new Room(n);
    }

    virtual Door *MakeDoor(Room *r1, Room *r2) const
    {
        return new Door(r1, r2);
    }


};

inline Maze * MazeGame(MazeFactory & factory)
{
    Maze *aMaze = factory.MakeMaze();
    Room *r1 = factory.MakeRoom(1);
    Room *r2 = factory.MakeRoom(2);
    Door *theDoor = factory.MakeDoor(r1, r2);

    aMaze->AddRoom(r1);
    aMaze->AddRoom(r2);

    r1->SetSide(North, factory.MakeWall());
    r1->SetSide(East, theDoor);
    r1->SetSide(South, factory.MakeWall());
    r1->SetSide(West, factory.MakeWall());

    r2->SetSide(North, factory.MakeWall());
    r2->SetSide(East, factory.MakeWall());
    r2->SetSide(South, factory.MakeWall());
    r2->SetSide(West, theDoor);

    return aMaze;
}
class Spell;
class DoorNeedingSpell;
class EnchchatedRoom;
class EnchantedMazeFactory : public MazeFactory
{
public:
    EnchantedMazeFactory();

    virtual Room *MakeRoom(int n) const
    {
        return new EnchchatedRoom(n , CastSpell());
    }

    virtual Door *MakeDoor(Room *r1, Room *r2) const
    {
        return new DoorNeedingSpell(r1, r2);
    }
protected:
    Spell *CastSpell() const;
    

};

}