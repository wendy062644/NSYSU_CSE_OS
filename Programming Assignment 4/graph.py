import numpy as np
import matplotlib.pyplot as plt

# 隨機生成請求數據
np.random.seed(0)  # 固定隨機種子以獲得可重現的結果
num_requests = 10
requests = np.random.randint(0, 200, size=num_requests)
initial_head_position = 100  # 初始磁頭位置

def plot_schedule(title, head_movements, requests):
    plt.figure(figsize=(10, 6))
    plt.plot(head_movements, marker='o')
    for i, txt in enumerate(head_movements):
        plt.annotate(txt, (i, head_movements[i]), textcoords="offset points", xytext=(0, 10), ha='center')
    plt.xticks(range(len(head_movements)))
    plt.xlabel('Steps')
    plt.ylabel('Cylinder')
    plt.title(title)
    plt.grid(True)
    plt.show()

def fcfs(requests, initial_head_position):
    head_movements = [initial_head_position] + list(requests)
    return head_movements

def sstf(requests, initial_head_position):
    requests = list(requests)
    head_position = initial_head_position
    head_movements = [head_position]
    while requests:
        closest_request = min(requests, key=lambda r: abs(r - head_position))
        head_movements.append(closest_request)
        head_position = closest_request
        requests.remove(closest_request)
    return head_movements

def scan(requests, initial_head_position, direction='left', lower_bound=0, upper_bound=200):
    requests = sorted(list(requests))
    head_position = initial_head_position
    head_movements = [head_position]
    if direction == 'left':
        left_requests = [r for r in requests if r <= head_position]
        right_requests = [r for r in requests if r > head_position]
        head_movements += sorted(left_requests, reverse=True) + [lower_bound] + sorted(right_requests)
    else:
        left_requests = [r for r in requests if r < head_position]
        right_requests = [r for r in requests if r >= head_position]
        head_movements += sorted(right_requests) + [upper_bound] + sorted(left_requests, reverse=True)
    return head_movements

def c_scan(requests, initial_head_position, lower_bound=0, upper_bound=200):
    requests = sorted(list(requests))
    head_position = initial_head_position
    head_movements = [head_position]
    right_requests = [r for r in requests if r >= head_position]
    left_requests = [r for r in requests if r < head_position]
    head_movements += sorted(right_requests) + [upper_bound, lower_bound] + sorted(left_requests)
    return head_movements


def look(requests, initial_head_position, direction='left'):
    requests = sorted(list(requests))
    head_position = initial_head_position
    head_movements = [head_position]
    if direction == 'left':
        left_requests = [r for r in requests if r <= head_position]
        right_requests = [r for r in requests if r > head_position]
        head_movements += sorted(left_requests, reverse=True) + sorted(right_requests)
    else:
        left_requests = [r for r in requests if r < head_position]
        right_requests = [r for r in requests if r >= head_position]
        head_movements += sorted(right_requests) + sorted(left_requests, reverse=True)
    return head_movements

def c_look(requests, initial_head_position):
    requests = sorted(list(requests))
    head_position = initial_head_position
    head_movements = [head_position]
    right_requests = [r for r in requests if r >= head_position]
    left_requests = [r for r in requests if r < head_position]
    head_movements += sorted(right_requests) + sorted(left_requests)
    return head_movements

# 模擬請求數據和初始磁頭位置
requests = np.random.randint(0, 200, size=num_requests)
initial_head_position = 100

# 運行並繪製每個演算法
plot_schedule('FCFS', fcfs(requests, initial_head_position), requests)
plot_schedule('SSTF', sstf(requests, initial_head_position), requests)
plot_schedule('SCAN', scan(requests, initial_head_position, direction='left'), requests)
plot_schedule('C-SCAN', c_scan(requests, initial_head_position), requests)
plot_schedule('LOOK', look(requests, initial_head_position, direction='left'), requests)
plot_schedule('C-LOOK', c_look(requests, initial_head_position), requests)
