from random import randint, choice

requests_number = 10
start_test_number = int(input("Enter start test number:\n"))
tests_number = int(input("Enter tests number:\n"))
max_number = int(input("Enter max number:\n"))

for i in range(start_test_number, start_test_number + tests_number):
    with open(f"./in/{i}test.in", "w") as test_file:
        for j in range(requests_number):
            request = choice(['k', 's k', 'r'])

            if request == 'r':
                test_file.write(f"r ")
                continue
            
            test_file.write(f"{request} {randint(1, max_number)} ")
