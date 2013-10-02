/*
 * init: The initial user-level program
 * For now, it spawns a shell, and that's
 * all.
 */

#include "types.h"
#include "include/stat.h"
#include "user.h"
#include "fcntl.h"

char *argv[] = { "sh", 0 };

int
main (void)
{
  int pid, 
      wpid;

  /* Here, init tries to create a new
   * console device, and then opens it
   * as file descriptors 0, 1, and 2
   */
  if (open ("console", O_RDWR) < 0) 
    {
      mknod ("console", 1, 1);
      open ("console", O_RDWR);
    }
  dup (0);  // stdout
  dup (0);  // stderr

  for (;;)
    {
      printf (1, "init: starting sh\n");
      pid = fork ();
      
      if (pid == 0)
        {
          exec ("sh", argv);

          /* [!!] This code gets executed if exec fails */
          printf (1, "init: exec sh failed\n");
          exit ();
        }
      else if (pid < 0) 
        {
          printf (1, "init: fork failed\n");
          exit ();
        }

      /* Here init handles orphaned zombies until the shell exits */
      while ((wpid = wait ()) >= 0 && wpid != pid)
        printf (1, "zombie!\n");
  }
}
