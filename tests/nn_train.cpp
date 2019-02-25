#include <iostream>

#include <dataset_images.h>
#include <classification_experiment.h>

int main()
{
	srand(time(NULL));

	DatasetImages dataset("dataset.json");

	{
		ClassificationExperiment experiment(dataset, "networks/net_0/");
		experiment.run();
	}
	{
		ClassificationExperiment experiment(dataset, "networks/net_1/");
		experiment.run();
	}
	{
		ClassificationExperiment experiment(dataset, "networks/net_2/");
		experiment.run();
	}
	/*
	{
		ClassificationExperiment experiment(dataset, "networks/net_3/");
		experiment.run();
	}
	*/
	std::cout << "program done\n";

 	return 0;
}
