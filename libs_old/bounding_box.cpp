#include <bounding_box.h>
#include <iostream>

BoundingBox::BoundingBox()
{
    m_class_id = 0;
    m_min_x = -1;
    m_max_x = -1;
    m_min_y = -1;
    m_max_y = -1;
    m_id = 0;
}

BoundingBox::BoundingBox(BoundingBox& other)
{
    copy(other);
}

BoundingBox::BoundingBox(const BoundingBox& other)
{
    copy(other);
}

BoundingBox::BoundingBox(int min_x, int max_x, int min_y, int max_y, unsigned int class_id, unsigned int id)
{
    m_class_id = class_id;
    m_min_x = min_x;
    m_max_x = max_x;
    m_min_y = min_y;
    m_max_y = max_y;
    m_id = id;
}

BoundingBox::~BoundingBox()
{

}

BoundingBox& BoundingBox::operator= (BoundingBox& other)
{
    copy(other);
    return *this;
}

BoundingBox& BoundingBox::operator= (const BoundingBox& other)
{
    copy(other);
    return *this;
}

void BoundingBox::set_extremal(int min_x, int max_x, int min_y, int max_y, unsigned int class_id, unsigned int id)
{
    if (m_min_x == -1)
    {
        m_class_id = class_id;
        m_min_x = min_x;
        m_max_x = max_x;
        m_min_y = min_y;
        m_max_y = max_y;
        m_id = id;
    }
    else
    if (m_id == id)
    {
        if (min_x < m_min_x)
            m_min_x = min_x;

        if (max_x > m_max_x)
            m_max_x = max_x;

        if (min_y < m_min_y)
            m_min_y = min_y;

        if (max_y > m_max_y)
            m_max_y = max_y;
    }
}

unsigned int BoundingBox::get_class_id()
{
    return m_class_id;
}

int BoundingBox::get_min_x()
{
    return m_min_x;
}

int BoundingBox::get_max_x()
{
    return m_max_x;
}

int BoundingBox::get_min_y()
{
    return m_min_y;
}

int BoundingBox::get_max_y()
{
    return m_max_y;
}

unsigned int BoundingBox::get_id()
{
    return m_id;
}

int BoundingBox::get_width()
{
    int res = m_max_x - m_min_x;
    return res;
}

int BoundingBox::get_height()
{
    int res = m_max_y - m_min_y;
    return res;
}

void BoundingBox::print()
{
    std::cout << get_id() << " ";
    std::cout << get_class_id() << " ";

    std::cout << "[";
    std::cout << get_min_x() << " " << get_max_x();
    std::cout << "]";

    std::cout << "[";
    std::cout << get_min_y() << " " << get_max_y();
    std::cout << "]";

    std::cout << "[";
    std::cout << get_width() << " " << get_height();
    std::cout << "]";

    std::cout << "\n";
}

void BoundingBox::copy(BoundingBox& other)
{
    m_class_id = other.m_class_id;

    m_min_x = other.m_min_x;
    m_max_x = other.m_max_x;
    m_min_y = other.m_min_y;
    m_max_y = other.m_max_y;
    m_id = other.m_id;
}

void BoundingBox::copy(const BoundingBox& other)
{
    m_class_id = other.m_class_id;

    m_min_x = other.m_min_x;
    m_max_x = other.m_max_x;
    m_min_y = other.m_min_y;
    m_max_y = other.m_max_y;
    m_id = other.m_id;
}
