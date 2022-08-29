1. With the .python-version file, this directory uses miniconda3-4.7.10
2. create an virtual environment for the project `conda create -n solace-test python=3.6`
3. install pip-tools from pip, run `pip install pip-tools`
4. create a `requirements.in` file which contains the direct packages for the project
5. run `pip-compile --output-file requirments.txt requirements.in`
6. run `pip-sync requirements.txt`




# Solace management port
http://localhost:8080
# login/pw
admin/admin
