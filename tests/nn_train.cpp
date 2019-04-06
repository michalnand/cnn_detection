#include <iostream>

#include <dataset_images.h>
#include <classification_experiment.h>

int main()
{
	srand(time(NULL));

	/*
	DatasetImages dataset("dataset_path.json");

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
	*/


	/*
	DatasetImages dataset("dataset_faces.json");

	{
		ClassificationExperiment experiment(dataset, "networks/faces_net_0/");
		experiment.run();
	}

	{
		ClassificationExperiment experiment(dataset, "networks/faces_net_1/");
		experiment.run();
	}

	{
		ClassificationExperiment experiment(dataset, "networks/faces_net_2/");
		experiment.run();
	}

	{
		ClassificationExperiment experiment(dataset, "networks/faces_net_3/");
		experiment.run();
	}
	*/


	DatasetImages dataset("dataset_mars.json");

	{
		ClassificationExperiment experiment(dataset, "networks/mars_net_0/");
		experiment.run();
	}

	std::cout << "program done\n";
 	return 0;
}
