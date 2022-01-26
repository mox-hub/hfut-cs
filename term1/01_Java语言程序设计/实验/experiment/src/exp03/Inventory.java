package exp03;

import com.sun.xml.internal.ws.api.ha.StickyFeature;
import org.omg.PortableInterceptor.SUCCESSFUL;
import sun.net.ftp.FtpClient;
import sun.security.krb5.internal.crypto.Des;

import java.io.*;
import java.util.*;

/**
 * 程序相关的文件一览
 * 库存记录：Inventory.txt
 * 事务文件：Transactions.txt
 * 发货记录：Shipping.txt
 * 错误信息：Errors.txt
 * 更新库存：NewInventory.txt
 */

public class Inventory {    //  程序开始
    //  公用的参数和流类
    FileReader fr;
    BufferedReader br;
    BufferedWriter bw;
    TransactionsData transactionsData;
    InventoryData inventoryData;
    List<InventoryData> InventoryList = new LinkedList<>();
    List<TransactionsData> TransactionsList = new LinkedList<>();

    //  构造函数的重构，包含程序处理的过程
    Inventory() {
        /**实现功能
         * 1.读取Inventory.txt的库存记录
         * 2.读取Transactions.txt的事务
         * 3.处理事务
         * 4.记录发货记录到Shipping.txt
         * 5.记录错误信息到Errors.txt
         * 6、更新信息到NewInventory.txt
         */
        readInventory();
        readTransactions();
        process();
        updateInventory();
    }

    //  读取Inventory.txt的库存记录
    private void readInventory() {
        try {
            String line;
            InventoryData inventoryData;
            /**InventoryData参数
             * String ItemNumber
             * int Quantity
             * String Supplier
             * String Description
             */
            //在try...catch里创建fr和br的实体
            fr = new FileReader("Inventory.txt");
            br = new BufferedReader(fr);

            //  循环录入inventoryData的数据
            while ((line = br.readLine()) != null) {
//                以空格分隔放入String数组中
                String[] split = line.trim().split("\\s+");
//                放入inventoryData对象中
                inventoryData = new InventoryData(split[0],
                        Integer.parseInt(split[1]), split[2], split[3]);
                InventoryList.add(inventoryData);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("读取Inventory.txt的库存记录成功");
    }

    //  读取Transactions.txt的事务
    private void readTransactions() {

        try {
            String line;
            //在try...catch里创建fr和br的实体
            fr = new FileReader("Transactions.txt");
            br = new BufferedReader(fr);

            //  循环录入inventoryData的数据
            while ((line = br.readLine()) != null) {
//                以空格分隔放入String数组中
                String[] split = line.trim().split("\\s+");
//                放入inventoryData对象中
                switch (line.charAt(0)) {
                    case 'O':
                        TransactionsList.add(new TransactionsData(split[0], split[1], Integer.parseInt(split[2]), split[3]));
                        break;
                    case 'R':
                        TransactionsList.add(new TransactionsData(split[0], split[1], Integer.parseInt(split[2])));
                        break;
                    case 'A':
                        TransactionsList.add(new TransactionsData(split[0], split[1], split[2], split[3]));
                        break;
                    case 'D':
                        TransactionsList.add(new TransactionsData(split[0], split[1]));
                        break;
                    default:
                        System.out.println("出错");
                }
                //TODO 2020年5月9日23:10:15
//                将信息添加到清单中

            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("Transactions.txt的事务读取成功");
    }

    //  处理事务
    private void process() {
        //TODO 2020年5月9日12:55:50 修改这里的发货订单的处理
        /**
         * 追加货物的处理(A)
         */
        for (int i = 0; i < TransactionsList.size(); i++) {
            if (TransactionsList.get(i).getType().equals("A")) {
//                获取transactionsData
                transactionsData = TransactionsList.get(i);
//                新建inventoryData储存新货物
                inventoryData = new InventoryData(transactionsData.getItemNumber(), 0,
                        transactionsData.getSupplier(), transactionsData.getDescription());
//                添加到列表中
                InventoryList.add(inventoryData);
            }
        }
        /**
         * 到货订单的处理(R)
         */
        for (int i = 0; i < TransactionsList.size(); i++) {

            if (TransactionsList.get(i).getType().equals("R")) {
//                比较订单和库存的ItemNumber，以进入下一步
                for (int j = 0; j < InventoryList.size(); j++) {
                    transactionsData = TransactionsList.get(i);
                    inventoryData = InventoryList.get(j);
                    if (inventoryData.getItemNumber() == transactionsData.getItemNumber()) {
//                        追加相应商品的数量
                        inventoryData.addQuantity(transactionsData.getQuantity());
                        InventoryList.set(j, inventoryData);
                    }
                }
            }
        }
        /**
         *发货订单的处理(O)
         */
        for (int i = 0; i < TransactionsList.size(); i++) {
            if (TransactionsList.get(i).getType().equals("O")) {
                for (int j = 0; j < InventoryList.size(); j++) {
                    inventoryData = InventoryList.get(j);
                    transactionsData = TransactionsList.get(i);
                    if (transactionsData.getItemNumber().equals(inventoryData.getItemNumber())) {
                        if (transactionsData.getQuantity() <= inventoryData.getQuantity()) {
//                        减少相应商品的数量
                            inventoryData.addQuantity(-transactionsData.getQuantity());
                            InventoryList.set(j, inventoryData);
//                        记录订单信息
                            ship(i);
                        } else {
                            error(i, 'O');
                        }
                    }
                }
            }
        }
        /**
         * 删除订单的处理(D)
         */
        for (int i = 0; i < TransactionsList.size(); i++) {
            if (TransactionsList.get(i).getType().equals("D")) {
                for (int j = 0; j < InventoryList.size(); j++) {
                    if (InventoryList.get(j).getItemNumber().equals(TransactionsList.get(i).getItemNumber())
                            && InventoryList.get(j).getQuantity() == 0) {
//                        删除此订单
                        InventoryList.remove(j);
                    } else {
                        error(j, 'D');
                    }
                }
            }
        }
        System.out.println("事务处理完毕");
    }

    //  记录发货记录到Shipping.txt
    private void ship(int i) {
        try {
            bw = new BufferedWriter(new FileWriter(".\\Shipping.txt", true));
            bw.write(TransactionsList.get(i).getSupplier() + "  ");
            bw.write(TransactionsList.get(i).getItemNumber() + "  ");
            bw.write(TransactionsList.get(i).getQuantity() + "  ");
            bw.newLine();
            bw.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("发货记录已登记...");
    }

    //  记录错误信息到Errors.txt
    private void error(int i, char Type) {
        try {
            bw = new BufferedWriter(new FileWriter(".\\Error.txt", true));

            if (Type == 'O') {
                bw.write(TransactionsList.get(i).getSupplier() + "\t");
                bw.write(TransactionsList.get(i).getItemNumber() + "\t");
                bw.write(TransactionsList.get(i).getQuantity() + "\t");
                bw.newLine();
            } else if (Type == 'D') {
                bw.write(0 + "\t");
                bw.write(InventoryList.get(i).getItemNumber() + "\t");
                bw.write(InventoryList.get(i).getQuantity() + "\t");
                bw.newLine();
            } else {
                System.out.println("发送error格式出错");
            }
            bw.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("错误已记录");
    }

    //  更新信息到NewInventory.txt
    private void updateInventory() {
        try {
            bw = new BufferedWriter(new FileWriter(".\\NewInventory.txt", true));
            for (int i = 0; i < InventoryList.size(); i++) {
                bw.write(InventoryList.get(i).getItemNumber() + "\t");
                bw.write(InventoryList.get(i).getQuantity() + "\t");
                bw.write(InventoryList.get(i).getSupplier() + "\t");
                bw.write(InventoryList.get(i).getDescription());
                bw.newLine();
            }
            bw.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("库存信息已更新完毕");
    }

    //  main函数
    public static void main(String[] args) {
        //TODO test here
        Inventory inventory = new Inventory();
    }
}

//用于储存一条信息的InventoryData和TransactionsData类
class InventoryData {
    private String ItemNumber;
    private int Quantity;
    private String Supplier;
    private String Description;

    InventoryData(String ItemNumber, int Quantity, String Supplier, String Description) {
        this.ItemNumber = ItemNumber;
        this.Quantity = Quantity;
        this.Supplier = Supplier;
        this.Description = Description;
    }

    public int getQuantity() {
        return Quantity;
    }

    public String getDescription() {
        return Description;
    }

    public String getItemNumber() {
        return ItemNumber;
    }

    public String getSupplier() {
        return Supplier;
    }

    public void addQuantity(int add) {
        this.Quantity += add;
    }
}

class TransactionsData {
    private String Type;
    private String ItemNumber;
    private int Quantity;
    private String Supplier;
    private String Description;

    //    case O
    TransactionsData(String Type, String ItemNumber, int Quantity, String Supplier) {
        this.Type = Type;
        this.ItemNumber = ItemNumber;
        this.Quantity = Quantity;
        this.Supplier = Supplier;
    }

    //    case R
    TransactionsData(String Type, String ItemNUmber, int Quantity) {
        this.Type = Type;
        this.ItemNumber = ItemNUmber;
        this.Quantity = Quantity;
    }

    //    case A
    TransactionsData(String Type, String ItemNumber, String Supplier, String Description) {
        this.Type = Type;
        this.ItemNumber = ItemNumber;
        this.Description = Description;
        this.Supplier = Supplier;
    }

    //    case D
    TransactionsData(String Type, String ItemNumber) {
        this.Type = Type;
        this.ItemNumber = ItemNumber;
    }

    public String getType() {
        return Type;
    }

    public int getQuantity() {
        return Quantity;
    }

    public String getDescription() {
        return Description;
    }

    public String getItemNumber() {
        return ItemNumber;
    }

    public String getSupplier() {
        return Supplier;
    }

}

