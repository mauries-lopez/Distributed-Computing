package com.example.database;

import org.springframework.data.mongodb.repository.MongoRepository;

import java.util.List;

public interface GradesRepository  extends MongoRepository<Grades, String> {

    List<Grades> findByEmail(String email);
}
