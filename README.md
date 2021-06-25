# Connect Four 

![Image](https://github.com/davidtpeng/connect-four-cpp/blob/master/connect_four_gui.png?raw=true)

This project allows the user to play Connect Four against a simple fully connected neural network using [alpha-beta search](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning). Simply click the screen to make a move and the computer opponent will automatically start to think.

Note: Setting a higher depth will result in a stronger opponent, but will cause it to move slower.

## Dependencies 
* [Visual Studio 2015+](https://visualstudio.microsoft.com/)
* [cmake](https://cmake.org/)
* [tiny-dnn](https://github.com/tiny-dnn/tiny-dnn) No installation needed, this can be copied into the project directory with the CMake file edited appropriately.
* [Cinder](https://libcinder.org/) 

## Build Instructions
### Windows
1) Download the Cinder library folder
2) Clone this project within the Cinder library folder
3) Download the tiny-dnn folder code and copy it into the Cinder library folder
4) Edit the CMakeLists.txt include_directories statement as necessary
5) Run the executables train-model (to train a model) and connect-four-simulator (to play against the model) in CLion or any other IDE

A similar process can be followed for other platforms, but has not yet been tested.

## Data
Two net binaries are provided in this project, net and net_2. net_2 is the stronger and default network that is loaded in the connect four executable.

net was trained on the following dataset: [UCI Machine Learning Repository](http://archive.ics.uci.edu/ml/datasets/connect-4)

In addition to the UCI dataset, net_2 was trained on this dataset of Connect Four positions: [Connect-Four Game Dataset](https://www.kaggle.com/tbrewer/connect-4) 

A model can be trained in the train-model executable by creating a data folder and copying the respective csv/data files into this folder. For reference, net_2 achieved a validation accuracy of roughly 84% when trained and tested on a combined dataset as described above.
