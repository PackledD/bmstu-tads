import tkinter as tk

c = 0


def check_first(a):
	return a[0]


def process_data(data):
	res = []
	for i in range(len(data) - 1):
		if data[i][0] != data[i + 1][0]:
			res.append(data[i])
	res.append(data[-1])
	return res


def draw_by_state(f, canv, flag):
	canv.delete("all")
	global c
	if flag:
		c += 1
	elif c != 0:
		c = c - 1
	if c == 0:
		return
	x_base = 1
	x_s = 120
	y_base = 1
	y_s = 30
	f.seek(0)
	data = []
	for i in range(c):
		data.append(f.readline().split('|'))
		if data[-1][0] == '':
			data.pop(-1)
			c -= 1
			break
		for j in range(len(data[-1])):
			if data[-1][j][-1] == '\n':
				data[-1][j] = data[-1][j][:-1]
		data[-1][1], data[-1][0] = data[-1][0], data[-1][1]
	data.sort(key=check_first)
	data = process_data(data)
	i = 0
	for y in range(10):
		for x in range(7):
			if x + y * 7 > len(data) - 1:
				return
			color = "lightgreen"
			if data[i][1] == "push" or data[i][1] == "alloc":
				color = "red"
			canv.create_rectangle(x_base + x * x_s, y_base + y * y_s, x_base + (x + 1) * x_s, y_base + (y + 1) * y_s, fill=color, outline="black")
			canv.create_text((x_base + 57 + x * x_s, y_base + 15 + y * y_s), text=data[i][0])
			i += 1


root = tk.Tk()
root.title("Memory checker")
root.geometry("840x350")
canv = tk.Canvas(root, width=840, height=310)
btns = tk.Frame(root)
btn_next = tk.Button(btns, text="-->", command=lambda: draw_by_state(f, canv, 1))
btn_prev = tk.Button(btns, text="<--", command=lambda: draw_by_state(f, canv, 0))
canv.grid(column=0, row=0)
btns.grid(column=0, row=1)
btn_next.grid(column=1, row=0)
btn_prev.grid(column=0, row=0)

f = open("log.txt", "r")
root.mainloop()
f.close()

