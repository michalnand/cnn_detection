#include <iostream>

#include <dataset_images.h>
#include <classification_experiment.h>

int main()
{
	srand(time(NULL));

	DatasetImages dataset("dataset_road.json");

	{
		ClassificationExperiment experiment(dataset, "networks/path_net_0/");
		experiment.run();
	}

	{
		ClassificationExperiment experiment(dataset, "networks/path_net_1/");
		experiment.run();
	}

	{
		ClassificationExperiment experiment(dataset, "networks/path_net_2/");
		experiment.run();
	}

	{
		ClassificationExperiment experiment(dataset, "networks/path_net_3/");
		experiment.run();
	}


	/*
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
	{
		ClassificationExperiment experiment(dataset, "networks/net_3/");
		experiment.run();
	}
	{
		ClassificationExperiment experiment(dataset, "networks/net_4/");
		experiment.run();
	}
	{
		ClassificationExperiment experiment(dataset, "networks/net_5/");
		experiment.run();
	}
	{
		ClassificationExperiment experiment(dataset, "networks/net_6/");
		experiment.run();
	}
	{
		ClassificationExperiment experiment(dataset, "networks/net_7/");
		experiment.run();
	}
	*/



	std::cout << "program done\n";
 	return 0;
}
