# Mini Shell

Find all `TODO`s in the existing code and complete the implementation of the shell. Remember what you've learned about using `fork`/`exec`/`wait`. Do not hesitate to the `man` page if you are unclear about an aspect of these function.

Do not forget that a shell must implement some commands a "builtin" functions. These functions are not executed in their own subprocess and instead their code is built into the shell itself. For example, `exit` and `cd` are often built into the shell, in addition to the notion of environment variables. **Why is this the case?**

There are two features you must implement in addition to building a basic shell:

 1. When a command's final argument is "&", do not wait for the job to complete. Instead, print the subprocess's PID and immediately prompt for another command input. The logic of this feature has already been implemented, you only must write the proper logic surrounding the `wait` call.
 2. Add a signal handler to this shell that catches `^C` (commonly known as `SIGINT`) and does not exit the program. The only way to quit your shell should be to run the `exit` builtin command. Do not forget that signal handles must be reinstalled after use.

## Testing

Run your shell from `bash` and execute various commands, ensuring they work.

## Using `git`

Remember to keep your repository history clean, with many small commits and descriptive commit messages. We want to be able to see the progression of your thoughts through the commit log, **not** just a single commit with all your code (and this will be reflected in your grade).

For the most part, the following commands will be all you need:
  - `git add`
  - `git commit`
  - `git status` (*always* use this before committing)
  - `git diff -stat`
  - `git push`
  - `git pull`
  - `git clone`

  You can learn about each of these using the internet, or `git help X` where `X` is the command portion of the above examples.
