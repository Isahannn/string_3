#include "pch.h"
#include <cctype>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <regex> // Подключаем библиотеку для работы с регулярными выражениями
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include <fstream>

// Функция для подсчета гласных в слове
int countVowels(const std::string& word) {
    int count = 0;
    for (char c : word) {
        char lower = std::tolower(c);
        if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u') {
            count++;
        }
    }
    return count;
}

// Функция для очистки слова от пунктуации с использованием регулярных выражений
std::string cleanWord(const std::string& word) {
    // Регулярное выражение для удаления пунктуации
    std::regex punctuationRegex(R"([^\w\s])");
    return std::regex_replace(word, punctuationRegex, ""); // Заменяем пунктуацию на пустую строку
}

// Функция для сортировки слов
std::vector<std::string> sortWords(const std::string& text) {
    std::istringstream stream(text);
    std::vector<std::string> words;
    std::string word;

    while (stream >> word) {
        words.push_back(cleanWord(word));
    }

    // Сортируем слова
    std::sort(words.begin(), words.end(), [](const std::string& a, const std::string& b) {
        if (a.length() != b.length()) {
            return a.length() > b.length(); // Сортировка по длине
        }
        int vowelsA = countVowels(a);
        int vowelsB = countVowels(b);
        if (vowelsA != vowelsB) {
            return vowelsA < vowelsB; // Сортировка по количеству гласных
        }
        return std::lexicographical_compare(
            a.begin(), a.end(),
            b.begin(), b.end(),
            [](char a, char b) { return std::tolower(a) < std::tolower(b); }
        ); // Лексикографическая сортировка
        });

    return words;
}

// Тесты для сортировки
TEST(SortingTest, LongestFirst) {
    std::string input = "The quick brown fox jumps over the lazy dog";
    std::vector<std::string> expected = { "brown", "jumps", "quick", "lazy", "over", "dog", "fox", "The", "the" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, MixedCase) {
    std::string input = "Hello world HELLO";
    std::vector<std::string> expected = { "world", "Hello", "HELLO" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, SpecialCharacters) {
    std::string input = "apple! banana? cherry.";
    std::vector<std::string> expected = { "cherry", "banana", "apple" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, NumbersInWords) {
    std::string input = "word1 word2 word10 word3";
    std::vector<std::string> expected = { "word10", "word1", "word2", "word3" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, AllSameLength) {
    std::string input = "cat bat hat";
    std::vector<std::string> expected = { "bat", "cat", "hat" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, DifferentLengths) {
    std::string input = "a ab abc abcd abcde";
    std::vector<std::string> expected = { "abcde", "abcd", "abc", "ab", "a" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, MixedSpaces) {
    std::string input = "This  is   a    test";
    std::vector<std::string> expected = { "test", "This", "is", "a" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, LongSentence) {
    std::string input = "The quick brown fox jumps over the lazy dog and the sun shines bright.";
    std::vector<std::string> expected = { "bright", "shines", "brown", "jumps", "quick", "lazy", "over", "and", "dog", "fox", "sun", "The", "the", "the" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, EmptyInput) {
    std::string input = "";
    std::vector<std::string> expected = {};
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

TEST(SortingTest, SingleWord) {
    std::string input = "hello";
    std::vector<std::string> expected = { "hello" };
    std::vector<std::string> result = sortWords(input);
    EXPECT_EQ(result, expected);
}

// Новый тест на существование файла
TEST(FileTests, InputFileExists) {
    std::ifstream file("input.txt");
    EXPECT_TRUE(file.good()); // Проверяем, что файл открыт успешно
}

// Новый тест на содержание файла
TEST(FileTests, InputFileContainsExpectedText) {
    std::ifstream file("input.txt");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string expectedContent = "The quick brown fox jumps over the lazy dog and the sun shines bright."; // Убедитесь, что текст соответствует
    EXPECT_EQ(content, expectedContent);
}

// Главная функция
int main(int argc, char** argv) {
    spdlog::stdout_color_mt("console");
    spdlog::get("console")->info("Starting the word sorting program...");

    ::testing::InitGoogleTest(&argc, argv);
    int testResult = RUN_ALL_TESTS();

    if (testResult == 0) {
        std::ifstream file("input.txt");
        if (!file) {
            spdlog::get("console")->error("Error opening file!");
            return 1;
        }

        std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        spdlog::get("console")->info("Processing text: {}", text);
        std::vector<std::string> sortedWords = sortWords(text);

        spdlog::get("console")->info("Sorted words:");
        for (const auto& w : sortedWords) {
            std::cout << w << " ";
        }
        std::cout << std::endl;
    }
    else {
        spdlog::get("console")->error("Tests failed. Exiting program.");
    }

    return testResult;
}