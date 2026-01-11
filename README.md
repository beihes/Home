# Home

这是一个用 **C++ + CMake** 构建的项目，主要用于展示某种应用逻辑（项目结构包含 app、engine、test、utils 等模块）。  
仓库使用 Git 进行版本管理，采用标准 CMake 编译体系组织代码。 :contentReference[oaicite:0]{index=0}

## 项目结构

```text
├── .vscode/                  # VSCode 配置
├── app/                      # 应用程序入口及实现
├── engine/                   # 核心逻辑模块
├── utils/                    # 通用工具代码
├── test/                     # 单元测试 / 测试用例
├── CMakeLists.txt            # 顶层 CMake 配置
├── .gitignore                # Git 忽略文件配置
└── …
