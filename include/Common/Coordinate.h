#pragma once
#include <iostream>

class Coordinate
{
private:
    int Row;
    int Column;

public:
    Coordinate();
    Coordinate(int const, int const);
    Coordinate(Coordinate const&);
    Coordinate(Coordinate &&) noexcept ;
    virtual ~Coordinate() = default;
    
    bool operator == (Coordinate const&) const;
    bool operator != (Coordinate const&) const;
    Coordinate& operator = (Coordinate const&);
    Coordinate& operator = (Coordinate&&) noexcept ;
    friend std::ostream& operator<<(std::ostream& out,const Coordinate& coordinate);
    
    int GetRow(void) const;
    int GetColumn(void) const;
};

struct CoordinateHasher
{
    std::size_t operator()(Coordinate const&) const noexcept;
};
