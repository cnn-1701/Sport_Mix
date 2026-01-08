#include"sport.h"
// 辅助函数
// 枚举转字符串
const char* GetProjectName(ProjectType type)
{
    switch (type)
    {
    case PROJECT_RUN_100:   return "100米跑";
    case PROJECT_RUN_400:   return "400米跑";
    case PROJECT_RUN_1000:  return "1000米跑";
    case PROJECT_SWIM:      return "游泳";
    case PROJECT_HIGH_JUMP: return "跳高";
    case PROJECT_LONG_JUMP: return "跳远";
    default:                return "未知项目";
    }
}

// 按项目查询菜单
void ProjectMenu(void)
{
    
    printf("*****   1. 100米跑\n");
    printf("*****   2. 400米跑\n");
    printf("*****   3. 1000米跑\n");
    printf("*****   4. 游泳\n");
    printf("*****   5. 跳高\n");
    printf("*****   6. 跳远\n");
}

// 报名选择项目的菜单
static void PrintProjectOptions(void)
{
    printf("*****   可选项目:\n");
    printf("*****   1. 100米跑\n")                                                                           ;
    printf("*****   2. 400米跑\n");
    printf("*****   3. 1000米跑\n");
    printf("*****   4. 游泳\n");
    printf("*****   5. 跳高\n");
    printf("*****   6. 跳远\n");
}

// 报名的项目数量有效验证
static int ValidCount(int count)
{
    return (count >= 1 && count <= MAX_PROJECTS_PER_STUDENT);
}

// 名次有效验证
static int ValidRank(int rank)
{

    return (rank >= 0 && rank <= 6);
}


// 名次转得分
static int GetScore(int rank)
{
    switch (rank)
    {
    case 1: 
        return SCORE_1ST;
    case 2: 
        return SCORE_2ND;
    case 3: 
        return SCORE_3RD;
    case 4:
        return SCORE_4TH;
    case 5: 
        return SCORE_5TH;
    case 6: 
        return SCORE_6TH;
    default: 
        return 0;
    }
}

// 按姓名查找
static int FindByName(const System* sys, const char* name)
{
    assert(sys && name);

    for (int i = 0; i < sys->count; i++)
    {
        if (strcmp(sys->students[i].name, name) == 0)
        {
            return i;
        }
    }
    return -1;
    // 无效姓名返回-1
}

// 项目名称查找
static int FindProject(const Student* stu, const char* projName)
{
    assert(stu && projName);

    for (int i = 0; i < stu->projectCount; i++)
    {

        if (strcmp(stu->projects[i].name, projName) == 0)
        {
            return i;
        }
    }
    return -1;
}

// 对学生.项目的结构体按项目名称排序
static void SortProjects(Student* stu)
{
    assert(stu);

    if (stu->projectCount <= 1)
        return;

    Project temp;
    for (int i = 0; i < stu->projectCount - 1; i++)
    {
        for (int j = 0; j < stu->projectCount - 1 - i; j++)
        {
            if (strcmp(stu->projects[j].name, stu->projects[j + 1].name) > 0)
            {
                temp = stu->projects[j];
                stu->projects[j] = stu->projects[j + 1];
                stu->projects[j + 1] = temp;
            }
        }
    }
}



// 初始化
void InitSystem(System* sys)
{
    assert(sys);
    sys->count = 0;
    sys->regStatus = STATUS_NOT_STARTED;
    sys->resultStatus = STATUS_NOT_STARTED;
    memset(sys->students, 0, sizeof(sys->students));

    LoadData(sys);

    // 根据加载的数据更新状态
    if (sys->count > 0)
    {
        sys->regStatus = STATUS_REGISTERED;
        // 检查是否有学生已录入成绩
        for (int i = 0; i < sys->count; i++)
        {
            for (int j = 0; j < sys->students[i].projectCount; j++)
            {
                if (sys->students[i].projects[j].rank > 0)
                {
                    sys->resultStatus = STATUS_RESULTS_INPUT;
                    break;
                }
            }
            if (sys->resultStatus == STATUS_RESULTS_INPUT)
                break;
        }
    }
}

// 从文件加载数据
void LoadData(System* sys)
{
    assert(sys);

    FILE* file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        return;
    }

    sys->count = 0;
    Student temp = { 0 };

    while (fread(&temp, sizeof(Student), 1, file))
    {
        if (sys->count >= MAX_STUDENTS)
        {
            printf("学生人数达到上限，停止加载\n");
            break;
        }
        sys->students[sys->count] = temp;
        sys->count++;
    }

    fclose(file);
    file = NULL;

    if (sys->count > 0)
    {
        printf("从文件加载 %d 了个学生数据\n", sys->count);
    }
}

// 保存数据到文件
void SaveData(const System* sys)
{
    assert(sys);

    FILE* file = fopen(FILENAME, "w");
    if (file == NULL)
    {
        perror("SaveData");
        return;
    }

    for (int i = 0; i < sys->count; i++)
    {
        fwrite(&sys->students[i], sizeof(Student), 1, file);
    }

    fclose(file);
    file = NULL;
}


// 按学号查找
static int FindById(const System* sys, const char* id)
{
    assert(sys && id);

    for (int i = 0; i < sys->count; i++)
    {
        if (strcmp(sys->students[i].id, id) == 0)
        {
            return i;
        }
    }
    return -1;
    // 无效学号返回-1
}


// 计算总分
static void CalcTotal(Student* stu)
{
    assert(stu);

    stu->totalScore = 0;
    for (int i = 0; i < stu->projectCount; i++)
    {
        stu->totalScore += stu->projects[i].score;
    }
}

// 报名功能
void RegisterStudent(System* sys)
{
    assert(sys);

    if (sys->count >= MAX_STUDENTS)
    {
        printf("录入的学生人数达到上限，学生\n");
        return;
    }

    printf("登记第 %d 位学生信息:\n", sys->count + 1);


    // 创建一个临时newstd对报名数据进行存储

    Student newStudent = { 0 };

    printf("请输入学号：>");
    scanf("%s", newStudent.id);

    // 检查学号
    if (FindById(sys, newStudent.id) != -1)
    {
        printf("该学号重复登记\n");
        return;
    }

    printf("请输入姓名：>");
    scanf("%s", newStudent.name);

    // 验证是否超过上限报名项目
    do
    {
        printf("请输入参赛项目数（1-%d）：>", MAX_PROJECTS_PER_STUDENT);
        scanf("%d", &newStudent.projectCount);
    } while (!ValidCount(newStudent.projectCount));

    // 打印可选项目菜单
    PrintProjectOptions();
    
    // 信息录入
    for (int i = 0; i < newStudent.projectCount; i++)
    {
        int choice;
        do
        {
            printf("请选择第 %d 个项目（1-6）：>", i + 1);
            scanf("%d", &choice);
        } while (choice < 1 || choice > PROJECT_COUNT - 1);

        // 枚举转字符存储
        strcpy(newStudent.projects[i].name, GetProjectName((ProjectType)choice));

        newStudent.projects[i].rank = 0;
        newStudent.projects[i].score = 0;
    }
    // 对学生录入的项目结构体排序
    SortProjects(&newStudent);

    CalcTotal(&newStudent);

    // 将处理完的结构体录入
    sys->students[sys->count] = newStudent;
    sys->count++;
    sys->regStatus = STATUS_REGISTERED;

    SaveData(sys);
    printf("***** 报名成功\n");
}

// 修改报名信息
void ModifyRgs(System* sys)
{
    assert(sys);

    if (sys->regStatus == STATUS_NOT_STARTED)
    {

        printf("尚未录入学生报名信息\n");
        return;
    }

    char id[MAX_ID] = { 0 };
    printf("请输入要修改学生学号：>");
    scanf("%s", id);

    int pos = FindById(sys, id);
    if (pos == -1)
    {
        printf("无该学号的学生报名\n");
        return;
    }
     
    printf("当前选手报名信息：\n");
    printf("选手姓名：%s\n", sys->students[pos].name);
    printf("选手学号：%s\n", sys->students[pos].id);

    if (sys->students[pos].projectCount > 0)
    {
        printf("报名项目：\n");
        for (int i = 0; i < sys->students[pos].projectCount; i++)
        {
            printf("第 %d 个项目：%s\n", i + 1, sys->students[pos].projects[i].name);
        }
    }

    // 选择修改类型
    int choice;
    do
    {
        printf("修改选项：\n");
        printf("1. 修改姓名\n");
        printf("2. 修改学号\n");
        printf("3. 修改报名项目\n");
        printf("请选择（1-3）：>");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 3);

    switch (choice)
    {
    case 1:
        printf("姓名修改为：>");
        scanf("%s", sys->students[pos].name);
        printf("**** 修改成功\n");
        break;

    case 2:
        printf("学号修改为：>");
        scanf("%s", sys->students[pos].id);
        printf("**** 修改成功\n");
        break;

    case 3:
        // 报名项目修改
        printf("重新输入参赛项目数（1-%d）：>", MAX_PROJECTS_PER_STUDENT);
        scanf("%d", &sys->students[pos].projectCount);

        if (!ValidCount(sys->students[pos].projectCount))
        {
            printf("项目数量无效！\n");
            return;
        }
        // 打印项目选择菜单
        PrintProjectOptions();
        for (int i = 0; i < sys->students[pos].projectCount; i++)
        {
            int projChoice;
            do
            {
                printf("请选择第 %d 个项目（输入1-6）：>", i + 1);
                scanf("%d", &projChoice);
            } while (projChoice < 1 || projChoice > PROJECT_COUNT - 1);

            strcpy(sys->students[pos].projects[i].name,GetProjectName((ProjectType)projChoice));
            sys->students[pos].projects[i].rank = 0;
            sys->students[pos].projects[i].score = 0;
        }
        printf("**** 修改成功\n");
        break;
    }

    SortProjects(&sys->students[pos]);
    CalcTotal(&sys->students[pos]);

    SaveData(sys);
}

// 录入比赛结果
void InputResult(System* sys)
{
    assert(sys);

    if (sys->regStatus == STATUS_NOT_STARTED)
    {
        printf("尚未录入学生报名信息\n");
        return;
    }

    printf("请选择录入方式：\n");
    printf("1. 按项目录入\n");
    printf("2. 按学生录入\n");
    printf("请选择（1-2）：>");

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        char targetProject[MAX_PROJECT_NAME] = { 0 };
        printf("请输入要录入结果的项目名称：>");
        scanf("%s", targetProject);

        int found = 0;
        for (int i = 0; i < sys->count; i++)
        {
            int projPos = FindProject(&sys->students[i], targetProject);
            if (projPos != -1)
            {
                found = 1;
                printf("请输入 %s（学号：%s）在 %s 项目的名次（1-6）：>",
                    sys->students[i].name,
                    sys->students[i].id,
                    targetProject);

                int rank;
                scanf("%d", &rank);

                if (!ValidRank(rank))
                {
                    printf("名次无效！\n");
                    continue;
                }

                sys->students[i].projects[projPos].rank = rank;
                sys->students[i].projects[projPos].score = GetScore(rank);
                CalcTotal(&sys->students[i]);
            }
        }

        if (!found)
        {
            printf("没有学生参加此项目\n");
            return;
        }
    }
    else if (choice == 2)
    {
        char stuId[MAX_ID] = { 0 };
        printf("请输入学生学号：>");
        scanf("%s", stuId);

        int pos = FindById(sys, stuId);
        if (pos == -1)
        {
            printf("未找到该学生\n");
            return;
        }

        printf("录入 %s 参加的项目排名与得分：\n", sys->students[pos].name);
        for (int i = 0; i < sys->students[pos].projectCount; i++)
        {
            printf("%s 排名：>", sys->students[pos].projects[i].name);
            scanf("%d", &sys->students[pos].projects[i].rank);

            if (!ValidRank(sys->students[pos].projects[i].rank))
            {
                printf("名次无效\n");
                continue;
            }

            printf("%s 得分：>", sys->students[pos].projects[i].name);

            sys->students[pos].projects[i].score = GetScore(sys->students[pos].projects[i].rank);


            printf("%d\n", sys->students[pos].projects[i].score);
        }
        CalcTotal(&sys->students[pos]);
    }
    else
    {
        printf("选择无效\n");
        return;
    }

    sys->resultStatus = STATUS_RESULTS_INPUT;
    SaveData(sys);
    printf("比赛结果录入完成\n");
}

// 查询功能
void SearchInfo(System* sys)
{
    assert(sys);

    if (sys->regStatus == STATUS_NOT_STARTED)
    {
        printf("报名尚未开始\n\n");
        return;
    }

    printf("请选择查询方式：\n");
    printf("1. 按姓名查询\n");
    printf("2. 按项目查询\n");
    printf("请选择：>");

    int choice;
    scanf("%d", &choice);

    if (choice == 1)
    {
        char name[MAX_NAME] = { 0 };
        printf("请输入要查询的学生姓名：>");
        scanf("%s", name);

        int pos = FindByName(sys, name);
        if (pos == -1)
        {
            printf("无该同学报名信息\n");
            return;
        }

        printf("该选手信息如下：\n");
        printf("姓名：%s\n", sys->students[pos].name);
        printf("学号：%s\n", sys->students[pos].id);
        printf("参赛项目：\n");

        for (int i = 0; i < sys->students[pos].projectCount; i++)
        {
            printf("第 %d 个项目：%s\n", i + 1, sys->students[pos].projects[i].name);

            if (sys->resultStatus == STATUS_RESULTS_INPUT && sys->students[pos].projects[i].rank > 0)
            {
                printf("%s 项目排名：%d\n",
                    sys->students[pos].projects[i].name,
                    sys->students[pos].projects[i].rank);
                printf("%s 项目得分：%d\n",
                    sys->students[pos].projects[i].name,
                    sys->students[pos].projects[i].score);
            }
            else
            {
                printf("尚未录入成绩\n");
            }
        }
    }
    else if (choice == 2)
    {
        ProjectMenu();
        printf("请选择项目（1-6）：>");

        int projChoice;
        scanf("%d", &projChoice);

        if (projChoice < 1 || projChoice > PROJECT_COUNT - 1)
        {
            printf("选择无效\n");
            return;
        }

        const char* projName = GetProjectName((ProjectType)projChoice);
        printf("%s 参赛选手：\n", projName);

        int found = 0;
        for (int i = 0; i < sys->count; i++)
        {
            for (int j = 0; j < sys->students[i].projectCount; j++)
            {
                if (strcmp(sys->students[i].projects[j].name, projName) == 0)
                {
                    found = 1;
                    printf("    %s ", sys->students[i].name);

                    if (sys->resultStatus == STATUS_RESULTS_INPUT && sys->students[i].projects[j].rank > 0)
                    {
                        printf("（排名：%d，得分：%d）",
                            sys->students[i].projects[j].rank,
                            sys->students[i].projects[j].score);
                    }
                    break;
                }
            }
        }

        if (!found)
        {
            printf("暂无选手参加该项目\n");
        }
        printf("\n");
    }
    else
    {
        printf("选择无效\n");
    }
}

// 删除报名信息
void DeleteStudent(System* sys)
{
    assert(sys);

    if (sys->regStatus == STATUS_NOT_STARTED)
    {
        printf("尚未录入报名信息\n");
        return;
    }

    char id[MAX_ID] = { 0 };
    printf("请输入要删除学生的学号：>");
    scanf("%s", id);

    int pos = FindById(sys, id);
    if (pos == -1)
    {
        printf("未找到该学号的学生\n");
        return;
    }

    printf("确认删除学生 %s（学号：%s）吗？(y/n)：>",
        sys->students[pos].name, sys->students[pos].id);
    char confirm = 'n';
    scanf(" %c", &confirm);

    if (confirm == 'y')
    {
        for (int i = pos; i < sys->count - 1; i++)
        {
            sys->students[i] = sys->students[i + 1];
        }
        sys->count--;

        if (sys->count == 0)
        {
            sys->regStatus = STATUS_NOT_STARTED;
            sys->resultStatus = STATUS_NOT_STARTED;
        }

        SaveData(sys);
        printf("删除成功\n");
    }
    else
    {
        printf("取消删除\n");
    }
}

// qsort的辅助比较函数
static int CompareScore(const void* e1, const void* e2)
{
    const Student* stu1 = (const Student*)e1;
    const Student* stu2 = (const Student*)e2;

    return stu2->totalScore - stu1->totalScore;
}
// 按总分对管理系统的学生成员排序
void SortByScore(System* sys)
{
    assert(sys);

    if (sys->regStatus == STATUS_NOT_STARTED)
    {
        printf("尚未录入报名信息\n");
        return;
    }

    if (sys->count == 0)
    {
        printf("暂无学生数据\n");
        return;
    }

    qsort(sys->students, sys->count, sizeof(Student), CompareScore);

    printf("总分排名：\n");
    printf("%-15s\t%-20s\t%-10s\t%-10s\n", "排名", "姓名", "学号", "总得分");

    for (int i = 0; i < sys->count; i++)
    {
        printf("%-15d\t%-20s\t%-10s\t%-10d\n",
            i + 1,
            sys->students[i].name,
            sys->students[i].id,
            sys->students[i].totalScore);
    }
}

// 打印所有学生的信息
void ShowAll(System* sys)
{
    assert(sys);

    if (sys->regStatus == STATUS_NOT_STARTED)
    {
        printf("尚未录入报名信息\n");
        return;
    }

    if (sys->count == 0)
    {
        printf("暂无学生数据\n");
        return;
    }

    printf("所有学生参赛信息：\n");
    printf("学生总数：%d\n", sys->count);
    printf("成绩状态：%s\n\n",sys->resultStatus == STATUS_RESULTS_INPUT ? "已录入" : "未录入");

    for (int i = 0; i < sys->count; i++)
    {
        printf("学生 %d：\n", i + 1);
        printf("姓名：%s\n", sys->students[i].name);
        printf("学号：%s\n", sys->students[i].id);
        printf("总得分：%d\n", sys->students[i].totalScore);

        if (sys->students[i].projectCount > 0)
        {
            printf("参赛项目：\n");
            printf("%-20s\t%-10s\t%-10s\n", "项目名称", "名次", "得分");

            for (int j = 0; j < sys->students[i].projectCount; j++)
            {
                printf("%-20s\t%-10d\t%-10d\n",
                    sys->students[i].projects[j].name,
                    sys->students[i].projects[j].rank,
                    sys->students[i].projects[j].score);
            }
        }
        printf("\n");
    }
}

// 管理员登录函数（简化版）
int Login(void)
{
    char password[50];
   
    printf("\n");
    printf("**************************************************\n");
    printf("******             运动会管理系统           ******\n");
    printf("**************************************************\n");

    while (1)
    {
        printf("请输入管理员密码：>");
        scanf("%s", password);

        if (strcmp(password,PASSWORD) == 0)
        {
            printf("\n登录成功\n");
            printf("**************************************************\n");
            return 1;  
        }
        else if (strcmp(password, "-1") == 0)
        {
            break;
        }
        else
        {
            printf("密码错误，请再次尝试\n");
            printf("输入-1以退出程序\n");
        }
    }
    return 0;
}