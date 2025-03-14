#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Empty Input Should Print Warning" {
    run ./dsh <<EOF

exit
EOF

    # Output should be a printed warning for empty inputs
    echo "Output: $output"
    [[ "$output" =~ "warning: no commands provided" ]]
    [ "$status" -eq 0 ]
}


@test "Should exit without problems" {
    run ./dsh <<EOF
exit
EOF

    # Exit with status of 0
    [ "$status" -eq 0 ]
}

@test "Multiple spaces between commands and arguments" {
    run ./dsh <<EOF
ls       -l
exit
EOF

    # exit should be 0
    [ "$status" -eq 0 ]
}

@test "multiple spaces at beginning of commands work" {
    run ./dsh <<EOF
     ls
exit
EOF

    # exit should be 0
    [ "$status" -eq 0 ]
}

@test "multiple spaces at end of commands work" {
    run ./dsh <<EOF
     ls
exit
EOF

    # exit should be 0
    [ "$status" -eq 0 ]
}

@test "cd without arguments should not change directory" {
    original_dir=$(pwd)  # Store the current directory
    run ./dsh <<EOF
cd
pwd
exit
EOF
    # Strip the prompt part (dsh3>) from the output
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/local mode//g')

    # Check if the current directory is still the same as before the cd command
    echo "Original: $original_dir"
    echo "Output: $stripped_output"
    [ "$stripped_output" = "$original_dir" ]
    [ "$status" -eq 0 ]
}

@test "cd with existing directory should change the directory" {
    run ./dsh <<EOF
cd /tmp
pwd
exit
EOF

    # Strip the prompt part (dsh3>) from the output
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/local mode//g')

    # Check if the output matches the new directory (e.g., /tmp)
    echo "Output: $stripped_output"
    [ "$stripped_output" = "/tmp" ]
    [ "$status" -eq 0 ]
}

@test "cd non-existing directory should not change directory" {
    original_dir=$(pwd)  # Store the current directory
    run ./dsh <<EOF
cd /nonexistingdir
pwd
exit
EOF

    # Strip the prompt part (dsh3>) from the output
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/local mode//g')

    # Check if the current directory is still the same as before the cd command
    echo "Original: $original_dir"
    echo "Output: $stripped_output"
    [ "$stripped_output" = "$original_dir" ]
    [ "$status" -eq 0 ]
}

@test "cd with .. should move up one directory level" {
    mkdir -p /tmp/testdir/subdir
    run ./dsh <<EOF
cd /tmp/testdir/subdir
pwd
exit
EOF
    # Strip the prompt part (dsh3>) from the output
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/local mode//g' )

    # Ensure the current directory is now /tmp/testdir
    echo "Output: $stripped_output"
    [ "$stripped_output" = "/tmp/testdir/subdir" ]
    [ "$status" -eq 0 ]
}

@test "exit command should be case sensitive" {
    run ./dsh <<EOF
EXIT
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]' | sed 's/localmode//g')

    # Expected output with all whitespace removed for easier matching
    expected_output="forkerrorinexecvp:Nosuchfileordirectorydsh4>dsh4>dsh4>cmdloopreturned0"

    # ensure fork error for case-sensitive command
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
}

@test "too many commands/pipes should return error" {
    run ./dsh <<EOF
ls | cat | cat | cat | cat | cat | cat | cat | cat
EOF
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]' | sed 's/localmode//g')

    # Expected error message
    expected_output="dsh4>error:pipinglimitedto8commandscmdloopreturned-2"

    # ensure it throws error for too many commands/pipes
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
}

@test "8 commands should work" {
    run ./dsh <<EOF
ls | cat | cat | cat | cat | cat | cat | cat
EOF
    # Strip the prompt part (dsh3>) from the output
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/cmd loop returned 0//g' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//' | sed 's/local mode//g')

    # Expected output from ls and stripping spaces
    expected_output=$(ls | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
}

@test "one command should work" {
    run ./dsh <<EOF
ls
EOF
    # Strip the prompt part (dsh3>) from the output
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/cmd loop returned 0//g' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//' | sed 's/local mode//g')

    # Expected output from ls and stripping spaces
    expected_output=$(ls | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')

    # ensure it has outputted correct ls
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
}

@test "echo args are outputted as is with extra spaces" {
    run ./dsh <<EOF
echo "    Hi,   this has extra spaces    "
EOF
    # Remove the prompt "dsh3> " and cmd loop return but keep the spaces in actual output
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/cmd loop returned 0//g' | sed 's/local mode//g')

    # Expected output with exact spaces
    expected_output="    Hi,   this has extra spaces    "

    # ensure it has outputted with extra spaces
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
}

@test "cat command should work with echo" {
    run ./dsh <<EOF
echo "Hello, this is a test" | cat
EOF
    # Remove the prompt "dsh3> " and cmd loop return
    stripped_output=$(echo "$output" | sed 's/dsh4> //g' | sed 's/cmd loop returned 0//g' | sed 's/local mode//g')

    # expected output with echo and cat
    expected_output="Hello, this is a test"

    # ensure correct output
    echo "${stripped_output} -> ${expected_output}"
    [ "$stripped_output" = "$expected_output" ]
}