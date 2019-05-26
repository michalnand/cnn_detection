#include <iostream>

#include <dataset_images.h>
#include <classification_experiment.h>

int main()
{
	srand(time(NULL));

	DatasetImages dataset("dataset_cells.json");

	{
		ClassificationExperiment experiment(dataset, "networks/cells_net_0/");
		experiment.run();
	}
 
	{
		ClassificationExperiment experiment(dataset, "networks/cells_net_1/");
		experiment.run();
	}



	std::cout << "program done\n";
 	return 0;
}
