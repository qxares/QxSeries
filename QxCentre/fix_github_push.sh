cd /home/ares/QxCentre
git rm --cached QxWrite
git add QxCentre QxWrite QxSheet
git commit -m "Fixed QxCentre/QxWrite/QxSheet build paths and removed QxWrite submodule"
git push --force origin main
ls -la
