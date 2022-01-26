public class Staff extends Employee {
    String ID;
    String name;
    String add;
    String Tel;
    String email;
    String position;
    String date;
//两个设置函数
    void setID(String ID) {
        this.ID = ID;
    }

    void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Staff{" +
                "ID='" + ID + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}
