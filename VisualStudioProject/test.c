#include"sport.h"

// 菜单显示函数 - 完全保持您的UI风格
void ShowMenu(void)
{
    printf("\n");
    printf("**************************************************\n");
    printf("******            运动会管理系统            ******\n");
    printf("**************************************************\n");
    printf("****** 1. 报名            2. 修改报名信息   ******\n");
    printf("****** 3. 录入比赛结果    4. 查询           ******\n");
    printf("****** 5. 删除            6. 按总得分排序   ******\n");
    printf("****** 7. 浏览所有        8. 保存并退出     ******\n");
    printf("****** 0. 直接退出                     *****\n");
    printf("******************************* ******************\n");
}

int main()
{
    if (!Login())
    {
        return 0;
    }

    int input = 0;
    System sys; 
    // 创建系统结构体变量

    // 初始化 + 导入文件信息
    InitSystem(&sys);

    printf("***** 欢迎使用运动会管理系统\n");

    do
    {
        ShowMenu();
        printf("请选择操作：>");
        scanf("%d", &input);

        switch (input)
        {
        case 1:
            RegisterStudent(&sys);
            break;
        case 2:
            ModifyRgs(&sys);
            break;
        case 3:
            InputResult(&sys);
            break;
        case 4:
            SearchInfo(&sys);
            break;
        case 5:
            DeleteStudent(&sys);
            break;
        case 6:
            SortByScore(&sys);
            break;
        case 7:
            ShowAll(&sys);
            break;
        case 8:
            SaveData(&sys);
            printf("已保存，退出程序\n");
            input = 0;
            break;
        case 0:
            printf("退出程序\n");
            break;
        default:
            printf("输入无效，请重新输入\n");
            break;
        }
    } while (input);

    return 0;
}