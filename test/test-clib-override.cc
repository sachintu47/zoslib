// Enable CLIB overrides
#define ZOSLIB_OVERRIDE_CLIB 1

#include "zos.h"
#include <fcntl.h>
#include <unistd.h>
#include "gtest/gtest.h"

namespace {

class CLIBOverrides : public ::testing::Test {
    virtual void SetUp() {
      // Make sure default untagged read mode is set
      setenv("UNTAGGED_READ_MODE", "AUTO", 1);
      temp_path = tmpnam(NULL);
      fd = open(temp_path, O_CREAT | O_WRONLY, 0660);
    }

    virtual void TearDown() {
      close(fd);
      remove(temp_path);
    }

protected:
    char* temp_path;
    int fd;
};

TEST_F(CLIBOverrides, open) {
    EXPECT_GE(fd, 0);
    // New files should be tagged as ASCII 819
    EXPECT_EQ(__getfdccsid(fd), 66355);

    // Should auto-convert untagged files to EBCDIC 1047
    fd = open("/etc/profile", O_RDONLY);
    if (fd >= 0) {
      EXPECT_EQ(__getfdccsid(fd), 66583);
      close(fd);
    }

    // Should not auto-convert character files
    fd = open("/dev/random", O_RDONLY);
    if (fd >= 0) {
      EXPECT_EQ(__getfdccsid(fd), 0);
      close(fd);
    }
}

TEST_F(CLIBOverrides, pipe) {
    int pipefd[2];
    int rc = pipe(pipefd);
    EXPECT_GE(rc, 0);
    EXPECT_GE(pipefd[0], 0);
    EXPECT_GE(pipefd[1], 0);
    EXPECT_EQ(__getfdccsid(pipefd[0]), 66355);
    EXPECT_EQ(__getfdccsid(pipefd[1]), 66355);

    int ccsid = __getfdccsid(STDOUT_FILENO);
    dup2(STDOUT_FILENO, pipefd[0]);
    EXPECT_EQ(__getfdccsid(pipefd[0]), ccsid);
}

TEST_F(CLIBOverrides, socketpair) {
    int fd[2];
    int rc = socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    EXPECT_GE(rc, 0);
    EXPECT_GE(fd[0], 0);
    EXPECT_GE(fd[1], 0);
    if (__is_os_level_at_or_above(ZOSLVL_V2R5)) {
      EXPECT_EQ(__getfdccsid(fd[0]), 65536);
      EXPECT_EQ(__getfdccsid(fd[1]), 65536);
    } else {
      EXPECT_EQ(__getfdccsid(fd[0]), 0);
      EXPECT_EQ(__getfdccsid(fd[1]), 0);
    }
}
} // namespace
