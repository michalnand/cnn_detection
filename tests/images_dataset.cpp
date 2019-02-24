#include <images_split.h>
#include <iostream>

int main()
{
    /*
    {
        ImagesSplit split("images_dataset_config/images_abstract.json");
    }

    {
        ImagesSplit split("images_dataset_config/images_city.json");
    }

    {
        ImagesSplit split("images_dataset_config/images_office.json");
    }

    {
        ImagesSplit split("images_dataset_config/images_industry.json");
    }
    */
    {
        ImagesSplit split("images_dataset_config/images_mountains.json");
    }



    std::cout << "program done\n";
    return 0;
}
