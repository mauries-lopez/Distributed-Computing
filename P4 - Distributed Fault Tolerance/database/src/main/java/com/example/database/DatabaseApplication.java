package com.example.database;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class DatabaseApplication implements CommandLineRunner {

	@Autowired
	private CourseRepository courseRepository;

	public static void main(String[] args) {
		SpringApplication.run(DatabaseApplication.class, args);
	}

	@Override
	public void run(String... args) throws Exception {

		courseRepository.deleteAll(); // Fresh restart of Courses

		courseRepository.save(new Course("C123A", "MAUI-THEORY", "Open", 10, 10));
		courseRepository.save(new Course("BA134", "DISTRIBUTED MAUI", "Open", 10, 10));
		courseRepository.save(new Course("D1934", "SYSTEM OF MAUI", "Open", 10, 10));
		courseRepository.save(new Course("FEQRE", "MARVEL ACCORD", "Open", 10, 10));
		courseRepository.save(new Course("PFOEQ", "VALOCALM", "Open", 10, 10));

		/*
		for (Course course : courseRepository.findAll()) {
			System.out.println(course);
		}
		 */
	}
}
