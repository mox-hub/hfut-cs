/*
 Navicat Premium Data Transfer

 Source Server         : local
 Source Server Type    : MySQL
 Source Server Version : 80013
 Source Host           : localhost:3306
 Source Schema         : order

 Target Server Type    : MySQL
 Target Server Version : 80013
 File Encoding         : 65001

 Date: 16/01/2023 18:51:22
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for order_table
-- ----------------------------
DROP TABLE IF EXISTS `order_table`;
CREATE TABLE `order_table`  (
  `order_id` bigint(10) NOT NULL AUTO_INCREMENT,
  `order_name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `supplier` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `item` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  `number` int(10) NULL DEFAULT NULL,
  `order_time` datetime NULL DEFAULT NULL,
  `end_time` datetime NULL DEFAULT NULL,
  `status` varchar(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NULL DEFAULT NULL,
  PRIMARY KEY (`order_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1231231237 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_0900_ai_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of order_table
-- ----------------------------
INSERT INTO `order_table` VALUES (1231231231, '图书订单', '新华书店', '三体2', 100, '2022-12-07 10:03:02', '2022-12-09 10:56:15', '0');
INSERT INTO `order_table` VALUES (1231231232, '电脑订单', '123', '戴尔2022', 22, '2022-11-29 13:06:03', '2022-12-05 13:06:08', '1');
INSERT INTO `order_table` VALUES (1231231233, '图书订单', '新华书店', '123', 12, '2022-12-03 13:22:33', '2022-12-08 13:22:39', '1');
INSERT INTO `order_table` VALUES (1231231235, '33', '123', '123', 1110, '2022-12-07 18:03:02', '2022-12-09 18:56:15', '0');
INSERT INTO `order_table` VALUES (1231231236, '123', '123', '123', 9, '2022-12-14 20:00:00', '2022-12-30 20:00:00', '1');

SET FOREIGN_KEY_CHECKS = 1;
