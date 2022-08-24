import pytest
import subprocess
import time
import os


def setup(self):
    os.system("rm -rf test.db")


def run_script(commands):
    p = subprocess.Popen("./db test.db", stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE, shell=True, encoding="gbk")

    return p.communicate(commands)

    # for cmd in commands:
    #     # print(cmd)
    #     p.stdin.write(cmd + "\n")
    #     p.stdin.flush()
    #     # time.sleep(0.1)

    # out = p.stdout.read()
    # err = p.stderr.read()

    # p.stdin.close()
    # p.stdout.close()
    # return (out,err)


@pytest.mark.skip(reason="no way of currently testing this")
def test_db():
    # commands = [
    #     "insert 1 user1 person1@example.com",
    #     "select",
    #     ".exit",
    # ]
    commands = (
        "insert 1 user1 person1@example.com\n"
        "select\n"
        ".exit\n"
    )
    outs, errs = run_script(commands)
    assert outs == (
        "db > Executed.\n"
        "db > (1, user1, person1@example.com)\n"
        "Executed.\n"
        "db > "
    )


@pytest.mark.skip(reason="no way of currently testing this")
def test_db_full():
    script = ""
    for i in range(1401):
        script = script + f"insert {i} user{i} person{i}@example.com\n"

    # script=script+"select\n"
    script = script+".exit\n"
    outs, errs = run_script(script)
    outs = outs + "\n" + errs
    outs = [out for out in outs.split('\n') if out]
    print(outs)
    # print(errs)
    assert outs[-2] == 'db > Error: Table full.'


@pytest.mark.skip(reason="no way of currently testing this")
def test_db_string_too_long():
    long_username = "a"*33
    long_email = "a"*256
    script = (
        f"insert 1 {long_username} {long_email}\n"
        "select\n"
        ".exit\n"
    )
    # print(script)
    outs, errs = run_script(script)
    # print(outs)
    assert outs == (
        "db > String is too long.\n"
        "db > Executed.\n"
        "db > "
    )


@pytest.mark.skip(reason="no way of currently testing this")
def test_db_id_negative():
    script = (
        "insert -1 cstack foo@bar.com\n"
        "select\n"
        ".exit\n"
    )
    # print(script)
    outs, errs = run_script(script)
    # print(outs)
    assert outs == (
        "db > ID must be positive.\n"
        "db > Executed.\n"
        "db > "
    )


@pytest.mark.skip(reason="no way of currently testing this")
def test_db_persistence():
    script1 = (
        "insert 1 user1 person1@example.com\n"
        ".exit\n"
    )

    outs1, errs1 = run_script(script1)
    # print(outs)
    assert outs1 == (
        "db > Executed.\n"
        "db > "
    )

    script2 = (
        "select\n"
        ".exit\n"
    )

    outs2, errs2 = run_script(script2)
    # print(outs)
    assert outs2 == (
        "db > (1, user1, person1@example.com)\n"
        "Executed.\n"
        "db > "
    )


def test_db_one_node_btree():
    script = ""
    for i in [3, 1, 2]:
        script = script + f"insert {i} user{i} person{i}@example.com\n"

    script = script+".btree\n"
    script = script+".exit\n"
    # print(script)
    out, err = run_script(script)
    # if err:
    #     out = out + "\n" + err

    print(out)
    assert out == (
        "db > Executed.\n"
        "db > Executed.\n"
        "db > Executed.\n"
        "db > Tree:\n"
        "leaf (size 3)\n"
        "  - 0 : 3\n"
        "  - 1 : 1\n"
        "  - 2 : 2\n"
        "db > "
    )


def test_db_constants():
    script = (
        ".constants\n"
        ".exit\n"
    )

    out, err = run_script(script)
    # if err:
    #     out = out + "\n" + err
    # print(outs)
    assert out == (
        "db > Constants:\n"
        "ROW_SIZE: 293\n"
        "COMMON_NODE_HEADER_SIZE: 6\n"
        "LEAF_NODE_HEADER_SIZE: 10\n"
        "LEAF_NODE_CELL_SIZE: 297\n"
        "LEAF_NODE_SPACE_FOR_CELLS: 4086\n"
        "LEAF_NODE_MAX_CELLS: 13\n"
        "db > "
    )