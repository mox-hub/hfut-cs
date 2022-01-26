public class Faculty extends Employee {
    String ID;
    String name;
    String add;
    String Tel;
    String email;
    String className;
    String classTime;
    String profInformation;
    //两个设置函数
    void setID(String ID) {
        this.ID = ID;
    }

    void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Faculty{" +
                "ID='" + ID + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}
