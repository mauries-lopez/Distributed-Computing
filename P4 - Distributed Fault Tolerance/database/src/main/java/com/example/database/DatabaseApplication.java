package com.example.database;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class DatabaseApplication implements CommandLineRunner {

	@Autowired
	private CourseRepository courseRepository;

	@Autowired
	private GradesRepository gradesRepository;
	@Autowired
	private StudentRepository studentRepository;

	public static void main(String[] args) {
		SpringApplication.run(DatabaseApplication.class, args);
	}

	@Override
	public void run(String... args) throws Exception {

		courseRepository.deleteAll(); // Fresh restart of Courses
		gradesRepository.deleteAll(); // Fresh restart of Grades
		studentRepository.deleteAll(); // Fresh restart of Student

		// Create Fake Courses

		courseRepository.save(new Course("C123A", "MAUI-THEORY", "Open", 10, 10));
		courseRepository.save(new Course("BA134", "DISTRIBUTED MAUI", "Open", 1, 10)); // 1 Avail Num Slot
		courseRepository.save(new Course("D1934", "SYSTEM OF MAUI", "Open", 10, 10));
		courseRepository.save(new Course("FEQRE", "MARVEL ACCORD", "Open", 10, 10));
		courseRepository.save(new Course("PFOEQ", "VALOCALM", "Open", 10, 10));


		// Create Fake Grades
		/*
		gradesRepository.save(new Grades("mauries_lopez@dlsu.edu.ph", "C123A", "MAUI-THEORY", "4.0"));
		gradesRepository.save(new Grades("mauries_lopez@dlsu.edu.ph", "FEQRE", "MARVEL ACCORD", "4.0"));
		gradesRepository.save(new Grades("mauries_lopez@dlsu.edu.ph", "BA134", "DISTRIBUTED MAUI", "4.0"));
		gradesRepository.save(new Grades("mauries_lopez@dlsu.edu.ph", "D1934", "SYSTEM OF MAUI", "4.0"));
		gradesRepository.save(new Grades("mauries_lopez@dlsu.edu.ph", "PFOEQ", "VALOCALM", "4.0"));
		*/

		/*
		for (Course course : courseRepository.findAll()) {
			System.out.println(course);
		}
		 */
	}
}
