package com.foocode.manager.controller;

import com.foocode.manager.model.Book;
import com.foocode.manager.service.BookService;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.*;

import javax.annotation.Resource;
import java.util.Map;

@CrossOrigin
@RestController
public class BookController {
    @Resource(name = "book")
    private BookService bookService;

    @GetMapping("/book/queryBook")
    public Object queryBook(@RequestParam Map<String,String> data) {
        return bookService.queryBook(data);
    }

    @GetMapping("/book/getBookByBookId")
    public Object getBookByBookId(@RequestParam Map<String,String> data) {
        return bookService.getBookByBookId(data);
    }

    @GetMapping("/book/getBookByClassId")
    public Object getBookByClassId(@RequestParam Map<String,String> data) {
        return bookService.getBookByClassId(data);
    }
    @GetMapping("/book/getAllBooks")
    public Object getAllBooks() {
        return bookService.getAllBooks();
    }

    @PostMapping("/book/addBook")
    public Object addBook(@RequestBody Book book) {
        return bookService.addBook(book);
    }

    @PostMapping("/book/editBook")
    public Object editBook(@RequestBody Book book) {
        return bookService.editBook(book);
    }

    @PostMapping("/book/deleteBook")
    public Object deleteBook(Long bookId){
        return bookService.deleteBook(bookId);
    }
}
