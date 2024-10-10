import numpy as np
import matplotlib.pyplot as plt

# 隨機生成請求數據
np.random.seed(3)  # 固定隨機種子以獲得可重現的結果
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

def Optimal(requests, initial_head_position, direction='left1'):
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

# 模擬請求數據和初始磁頭位置
requests = np.random.randint(0, 200, size=num_requests)
initial_head_position = 100

# 運行並繪製每個演算法
plot_schedule('Optimal', Optimal(requests, initial_head_position), requests)
