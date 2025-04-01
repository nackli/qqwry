# IP 地理信息解析库 - QQWry.Dat 增强版

# 【版权风险提示】
⚠️ 特别声明：
• qqwry.dat 文件版权归属纯真网络所有
• 使用者需确保数据文件的合法获取及合规使用
• 本项目不承担因数据文件引发的版权纠纷责任

# 【项目状态通告】
根据官方公告，纯真网络将于 2024 年 10 月正式停止维护 qqwry.dat 格式数据库。本仓库将持续维护历史版本数据，并提供技术解析方案供开发者参考使用。最新数据请通过纯真网络官方渠道获取。

# 【数据文件说明】
[qqwry.dat](https://raw.githubusercontent.com/nackli/qqwry/main/qqwry.dat) 
│实时更新直链（覆盖更新机制）
│数据来源：纯真网络官方每日构建（非安装包格式）
├[Releases](https://github.com/nackli/qqwry/releases)
│归档历史版本数据（2020-2024全量版本）

# 【技术特性】
采用内存映射（Memory-Mapped File）技术实现高效解析：
✓ 零拷贝数据访问 - 直读磁盘文件无需全量加载
✓ 毫秒级响应 - 百万次查询/秒的处理性能
✓ 低内存占用 - 常驻内存≤4MB
✓ 多线程安全 - 支持高并发访问

# 【数据覆盖说明】
当前版本支持：

中国大陆 IP 段精准定位（区县级）

基础运营商信息解析

ASN 网络标识识别

已知限制：
× 港澳台地区定位精度受限
× 海外 IP 仅显示国家级信息
× 部分云服务商 IP 识别偏差

# 【使用建议】

建议每周通过官方渠道更新数据文件

海外 IP 推荐配合 MaxMind 数据库使用

关键业务建议建立数据版本回滚机制

# 【开源声明】
本解析引擎采用 MIT 开源协议，开发者可免费用于：

商业/非商业项目

二次开发及定制化改造

学术研究及技术验证

至于qqwry.dat文件的版本信息，请自行查找。本项目只是技术交流等，如果涉及到版权问题，请自行处理

# 【技术交流】
欢迎通过以下方式参与社区建设：
✉️ 核心维护者：nackli@163.com
📝 GitHub Issues 跟踪
💬 开发者讨论群组（申请入群请邮件联系）

版本更新记录：2024.08.15 优化索引结构，提升 30% 查询性能

注：本项目与纯真网络无商业关联，数据文件版权归属原权利方所有。


