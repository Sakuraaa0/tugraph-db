#include <iostream>
#include <string>
#include "TuGraphClient.h"  // TuGraph C++ SDK

int main() {
    // TuGraph 的服务器地址，用户名和密码
    std::string server_url = "http://localhost:7000";
    std::string username = "admin";
    std::string password = "password";

    // 创建 TuGraph 客户端实例
    TuGraph::Client client(server_url, username, password);

    // 要操作的数据库名称
    std::string db_name = "Facebook";

    // 连接到图数据库
    if (!client.Connect()) {
        std::cerr << "Failed to connect to TuGraph server!" << std::endl;
        return -1;
    }

    // 设置 PageRank 算法的参数
    int max_iter = 20;           // 最大迭代次数
    double damping_factor = 0.85; // 阻尼系数，通常设为 0.85
    double epsilon = 1e-6;       // 收敛阈值

    // 创建 Cypher 查询来调用 PageRank 算法
    std::string query = R"(
        CALL algo.pageRank($max_iter, $damping_factor, $epsilon)
        YIELD node, rank
        RETURN node, rank
    )";

    // 设置查询参数
    std::map<std::string, TuGraph::Value> params;
    params["max_iter"] = TuGraph::Value::Int(max_iter);
    params["damping_factor"] = TuGraph::Value::Double(damping_factor);
    params["epsilon"] = TuGraph::Value::Double(epsilon);

    // 执行查询
    auto result = client.CallCypher(db_name, query, params);

    // 检查是否执行成功
    if (!result.success) {
        std::cerr << "Error executing PageRank: " << result.error_message << std::endl;
        return -1;
    }

    // 输出结果
    for (const auto& record : result.records) {
        std::string node = record["node"].ToString();  // 获取节点 ID
        double rank = record["rank"].ToDouble();        // 获取 PageRank 值
        std::cout << "Node ID: " << node << ", PageRank: " << rank << std::endl;
    }

    return 0;
}
