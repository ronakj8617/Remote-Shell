#!/bin/bash
set -e

# Show GUI input box using zenity
COMMIT_MSG=$(zenity --entry --title="Git Commit" --text="Enter commit message:")

# Exit if no message entered
if [ -z "$COMMIT_MSG" ]; then
    echo "Commit message is empty. Aborting."
    exit 1
fi

git add .
git commit -m "$COMMIT_MSG"
git push
