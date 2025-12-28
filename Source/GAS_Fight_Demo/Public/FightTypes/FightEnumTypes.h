#pragma once

/**
 * 确认类型枚举，用于扩展蓝图节点的执行引脚
 * 通过 ExpandEnumAsExecs 元数据，可以根据函数返回值提供不同的执行路径
 */
UENUM()
enum class EFightConfirmType : uint8
{
	Yes,
	No
};

/**
 * 定义有效性类型枚举，用于表示对象是否有效
 * 主要用于检查对象是否存在或是否有效，配合ExpandEnumAsExecs元数据在蓝图中扩展执行引脚
 */
UENUM()
enum class EFightValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EFightSuccessType : uint8
{
	Successful,
	Failed
};