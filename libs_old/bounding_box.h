#ifndef _BOUNDING_BOX_H_
#define _BOUNDING_BOX_H_

class BoundingBox
{
    public:
      BoundingBox();
      BoundingBox(BoundingBox& other);
      BoundingBox(const BoundingBox& other);
      BoundingBox(int min_x, int max_x, int min_y, int max_y, unsigned int class_id, unsigned int id = 0);

      virtual ~BoundingBox();

      BoundingBox& operator= (BoundingBox& other);
      BoundingBox& operator= (const BoundingBox& other);

    public:
        void set_extremal(int min_x, int max_x, int min_y, int max_y, unsigned int class_id, unsigned int id = 0);

        unsigned int get_class_id();

        int get_min_x();
        int get_max_x();
        int get_min_y();
        int get_max_y();

        unsigned int get_id();

        int get_width();
        int get_height();
        void print();

    protected:
      void copy(BoundingBox& other);
      void copy(const BoundingBox& other);

    private:
        unsigned int m_class_id;
        int m_min_x, m_max_x, m_min_y, m_max_y;
        unsigned int m_id;
};

#endif
