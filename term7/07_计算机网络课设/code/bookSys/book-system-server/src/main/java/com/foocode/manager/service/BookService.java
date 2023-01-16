package com.foocode.manager.service;

import com.foocode.manager.model.Book;

import java.util.Map;

public interface BookService {

    Object addBook(Book book);

    Object deleteBook(Long bookId);

    Object editBook(Book book);

    Object getAllBooks();

    Object getBookByBookId(Map<String,String> data);

    Object getBookByClassId(Map<String,String> data);

    Object queryBook(Map<String,String> data);

}
