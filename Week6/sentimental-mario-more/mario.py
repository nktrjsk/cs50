# TODO
while True:
    try:
        height = int(input("Height: "))
    except ValueError:
        print("Not an integer")
    else:
        if 8 >= height >= 1:
            break

for i in range(1, height+1):
    print(" " * (height - i) + "#" * i + "  " + "#" * i)