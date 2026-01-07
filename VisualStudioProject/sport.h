#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define PASSWORD "123456"  // 管理员密码



#define MAX_STUDENTS 100
#define MAX_PROJECTS_PER_STUDENT 3
#define MAX_NAME 20
#define MAX_ID 15
#define MAX_PROJECT_NAME 30
#define FILENAME "data/sports_data.txt"


// 查询状态
#define STATUS_NOT_STARTED 0// 未报名
#define STATUS_REGISTERED 1// 已报名
#define STATUS_RESULTS_INPUT 2// 报名且比赛信息录入了

// 项目类型枚举
typedef enum ProjectType
{
    PROJECT_RUN_100 = 1,
    PROJECT_RUN_400,
    PROJECT_RUN_1000,
    PROJECT_SWIM,
    PROJECT_HIGH_JUMP,
    PROJECT_LONG_JUMP,
    PROJECT_COUNT
} ProjectType;


// 得分宏定义
#define SCORE_1ST 7
#define SCORE_2ND 5
#define SCORE_3RD 4
#define SCORE_4TH 3
#define SCORE_5TH 2
#define SCORE_6TH 1

// 项目结构体
typedef struct ProjectInfo
{
    char name[MAX_PROJECT_NAME];
    int rank;
    int score;
} Project;

// 学生结构体
// 学号、姓名、项目、项目数量、总分
typedef struct StudentInfo
{
    char id[MAX_ID];
    char name[MAX_NAME];
    Project projects[MAX_PROJECTS_PER_STUDENT];
    int projectCount;
    int totalScore;
} Student;

// 运动会管理系统结构体
typedef struct SportsSystem
{
    Student students[MAX_STUDENTS];
    int count;
    int regStatus;
    int resultStatus;
} System;
// 大致结构为：管理系统(学生数量、报名状态、成绩状态).学生(学号、姓名、项目数量、总分).项目

// 初始化比赛信息系统
void InitSystem(System* sys);

// 报名
void RegisterStudent(System* sys);

// 修改
void ModifyRgs(System* sys);

// 录入比赛名词
void InputResult(System* sys);

// 查询
void SearchInfo(System* sys);

// 删除
void DeleteStudent(System* sys);

// 按总得分排序
void SortByScore(System* sys);

// 打印所有信息
void ShowAll(System* sys);

// 保存文件
void SaveData(const System* sys);

// 读取文件
void LoadData(System* sys);

// 主菜单
void ShowMenu(void);

// 报名时项目选择菜单
// 这里不是查询菜单
void ShowProjectMenu(void);

// 枚举转为字符串
const char* GetProjectName(ProjectType type);

// 管理员登录
int Login(void);
