#include <iostream>

#include <dataset_detection.h>
#include <classification_experiment.h>

int main()
{
	srand(time(NULL));

	DatasetDetection dataset("dataset_letters_detection.json");

	//dataset.save_images("dataset_samples/training/", "dataset_samples/testing/");


	{
		ClassificationExperiment experiment(dataset, "networks/letter_net_0/");
		experiment.run();
	}

	{
		ClassificationExperiment experiment(dataset, "networks/letter_net_1/");
		experiment.run();
	}


	std::cout << "program done\n";

 	return 0;
}
