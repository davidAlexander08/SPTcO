#!/bin/bash

# Update system
sudo yum update -y

# Install Development Tools
sudo yum groupinstall "Development Tools" -y

# Install required libraries
sudo yum install -y cmake bzip2-devel readline-devel openmpi openmpi-devel htop

# Configure OpenMPI paths
echo 'export PATH=/usr/lib64/openmpi/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/lib64/openmpi/lib:$LD_LIBRARY_PATH' >> ~/.bashrc

# Apply new environment variables to current session
source ~/.bashrc

# Clone the repository
git clone https://github.com/davidAlexander08/SPTcO.git

# Prepare ThirdParty directory
cd SPTcO/ThirdParty
tar -xzvf lnxCoinInstaller.tar.gz
./lnxCoinInstaller.sh


echo "✅ Setup complete."
