public class Student extends Person {
    String add;
    String Tel;
    String email;
    String classInSchool;
    String stage;
//两个设置函数
    void setID(String ID) {
        this.ID = ID;
    }

    void setName(String name) {
        this.name = name;
    }

    @Override
    public String toString() {
        return "Student{" +
                "ID='" + ID + '\'' +
                ", name='" + name + '\'' +
                '}';
    }
}
