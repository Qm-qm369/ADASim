# -*- coding: utf-8 -*-

import socket
import json

# =========================================
# V0.10 简化 Lattice Planner
# =========================================
HOST = "127.0.0.1"
PORT = 8080

# 5个候选横向偏移
CANDIDATE_OFFSETS = [
    -3.5,
    -1.75,
    0.0,
    1.75,
    3.5
]
# 只关心前方25米
MAX_LOOKAHEAD = 25.0
# 与障碍物至少保持的横向距离
SAFE_LATERAL_DISTANCE = 1.2

# 三种代价的权重
W_LANE = 2.0
W_SMOOTH = 3.0
W_COLLISION = 1000.0


def evaluate_lattice_cost(target_offset, obstacles, last_offset):
    """
    计算一条候选轨迹的总代价。
    """
    cost = 0.0

    # =====================================
    # 1. 偏离道路中心的代价
    # =====================================
    cost += W_LANE * abs(target_offset)

    # =====================================
    # 2. 与上一帧差异的平滑代价
    # =====================================
    cost += W_SMOOTH * abs(target_offset - last_offset)

    # =====================================
    # 3. 障碍物碰撞代价
    # =====================================
    for obstacle in obstacles:
        x = float(obstacle.get("local_x", 0.0))
        y = float(obstacle.get("local_y", 0.0))

        # 后方障碍物不考虑
        if x <= 0.0:
            continue
        # 太远的不考虑
        if x > MAX_LOOKAHEAD:
            continue

        lateral_gap = abs(y - target_offset)
        # 距离过近：给一个非常大的碰撞代价
        if lateral_gap < SAFE_LATERAL_DISTANCE:
            cost += W_COLLISION * (SAFE_LATERAL_DISTANCE - lateral_gap + 1.0)
        else:
            # 虽然没有撞上，但是距离越近仍然略微惩罚
            cost += 5.0 / (lateral_gap + 0.1)

    return cost


def choose_best_offset(obstacles, last_offset):
    """
    对5个候选轨迹逐一打分，
    返回总代价最低的offset。
    """
    best_offset = 0.0
    best_cost = float("inf")

    for offset in CANDIDATE_OFFSETS:
        cost = evaluate_lattice_cost(offset, obstacles, last_offset)
        print("offset={:5.2f}, cost={:8.2f}".format(offset, cost))
        if cost < best_cost:
            best_cost = cost
            best_offset = offset

    return best_offset


def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("正在连接 ADASim...")
    client.connect((HOST, PORT))
    print("已连接 ADASim TCP Server")

    buffer = ""
    last_offset = 0.0

    while True:
        data = client.recv(4096)
        if not data:
            print("ADASim连接已关闭")
            break

        buffer += data.decode("utf-8")
        # 一行就是一条完整JSON
        while "\n" in buffer:
            line, buffer = buffer.split("\n", 1)
            if not line.strip():
                continue

            message = json.loads(line)
            if message.get("type") != "OBSTACLES":
                continue

            obstacles = message.get("data", [])
            print("\n收到障碍物数量:", len(obstacles))
            best_offset = choose_best_offset(obstacles, last_offset)
            print("选择轨迹:", best_offset)
            last_offset = best_offset

            response = {
                "type": "CONTROL",
                "steer_offset": best_offset
            }
            client.sendall((json.dumps(response) + "\n").encode("utf-8"))

    client.close()


if __name__ == "__main__":
    main()
