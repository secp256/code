#!/bin/bash

read -p "Login: " username
read -p "Passwd: " password

echo $username
echo $password

bash -c eyerest-daemon
