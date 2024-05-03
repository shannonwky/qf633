# QF633-C-for-Financial-Engineering
This repository is for the purpose of fulfilling QF633 group project requirements under the Quantitative Finance masters with Singapore Management University. It is strictly for educational purposes and does not constitute financial advice nor endorsement of any financial exchanges.

## 1. Creating your Personal Access Token (PAT)
Github now uses PAT rather than account password as a form of verification. To generate your own PAT:

1. Click on your profile picture at the top right of the screen
2. Select settings from the sidebar menu
3. Scroll down until you locate developer settings on the right side of the screen
4. Select Fine-grained token under Personal access tokens
5. Generate a new token with the following permissions \
    a. Expiration: 60 days \
    b. Repository access: Only select repositories > QF633-C-for-Financial-Engineering repository \
    c. Permissions: Set to Read and write for Actions, Administration, Commit Statuses, Contents, Issues and Pull Requests
6. Save the generated token someplace save for future verification if required

## 2. Cloning the repository
From the `terminal`, navigate to the desired location to store the project repository using the command `cd`. You may check the available directories/folders to navigate into using the `ls` command, and the `cd ..` command to move up one level. Once in the desired location, clone the repository by keying in the following command:
```
git clone https://github.com/xueyuanhuang/QF633-C-for-Financial-Engineering.git
```
The terminal should prompt you to enter your github username and password. The password here refers to the PAT generated in the earlier step, not your account password.

## 3. Syncing the repository
To ensure that the repository remains the latest version prior to making any new commits/edits, use the command:
```
git checkout main
git pull
```

## 4. Making and committing changes to the repository
Any code implementation/changes should be done on a separate branch before merging into the `main` branch to avoid potential conflicts. To create a new branch:
```
git checkout -b <short-branch-name>
```
<.> should be replaced with your desired branch name. To avoid branches having similar names, try to name it something related to the portion of the project you are doing. Check that the terminal reflects the name of the branch you are in prior to writing your new code.

Once your code is ready, follow steps 1-3 to commit the new codes into the main branch:
```
# 1a. To commit individual files
git add <filename>

# 1b. To commit all changes made since the last update
git add .

# 2. Description covering what the commit is about
git commit -m 'short descriptive message about the commit'

# 3. Commit the changes to the main repository
git push -u origin <short-branch-name>

# 4. Switch back to the main branch
git checkout main
```
The new files/folders will be uploaded into the repository once the pull request has been approved.
