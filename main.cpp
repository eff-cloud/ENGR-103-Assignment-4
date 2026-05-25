/*
 * Author: Ella Farrell
 * Description: Terminal dictionary interface using the
 * Free Dictionary API. Users can retrieve definitions,
 * audio URLs, and pronunciations of words.
 */

#include <string>
#include <sstream>
#include <iostream>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp-httplib/httplib.h"

#include "json11/json11.hpp"

const std::string DICTIONARY_API_HOST =
  "https://api.dictionaryapi.dev";

const std::string DICTIONARY_API_PATH_PREFIX =
  "/api/v2/entries/en/";

using json11::Json;

/*
 * Function: build_request_endpoint
 * Description: Builds API request endpoint.
 */
std::string build_request_endpoint(std::string word)
{
  std::stringstream endpoint_ss;

  endpoint_ss << DICTIONARY_API_PATH_PREFIX;
  endpoint_ss << word;

  return endpoint_ss.str();
}

/*
 * Function: extract_definition
 * Description: Extracts first available definition.
 */
std::string extract_definition(const Json words)
{
  const Json::array &word_arr =
    words.array_items();

  for (const Json &word : word_arr) {
    const Json &meanings =
      word["meanings"];

    const Json::array &meaning_arr =
      meanings.array_items();

    for (const Json &meaning : meaning_arr) {
      const Json thesaurus_entries =
        meaning["definitions"];

      const Json::array &
        thesaurus_entry_arr =
        thesaurus_entries.array_items();

      for (
        const Json &thesaurus_entry :
        thesaurus_entry_arr
      ) {
        std::string definition =
          thesaurus_entry["definition"]
          .string_value();

        if (!definition.empty()) {
          return definition;
        }
      }
    }
  }

  return "";
}

/*
 * Function: query_definition
 * Description: Queries definition of word.
 */
std::string query_definition(std::string word)
{
  httplib::Client cli(
    DICTIONARY_API_HOST
  );

  auto res =
    cli.Get(
      build_request_endpoint(word)
    );

  if (res->status != 200) {
    return "";
  }

  std::string err;

  Json words =
    Json::parse(
      res->body,
      err
    );

  if (!err.empty()) {
    return "";
  }

  return extract_definition(words);
}

/*
 * Function: extract_audio_url
 * Description: Extracts first audio URL.
 */
std::string extract_audio_url(
  const Json words
)
{
  const Json::array &word_arr =
    words.array_items();

  for (const Json &word : word_arr) {
    const Json phonetics =
      word["phonetics"];

    const Json::array &
      phonetic_arr =
      phonetics.array_items();

    for (
      const Json &phonetic :
      phonetic_arr
    ) {
      std::string audio_url =
        phonetic["audio"]
        .string_value();

      if (!audio_url.empty()) {
        return audio_url;
      }
    }
  }

  return "";
}

/*
 * Function: query_audio_url
 * Description: Queries audio URL.
 */
std::string query_audio_url(
  std::string word
)
{
  httplib::Client cli(
    DICTIONARY_API_HOST
  );

  auto res =
    cli.Get(
      build_request_endpoint(word)
    );

  if (res->status != 200) {
    return "";
  }

  std::string err;

  Json words =
    Json::parse(
      res->body,
      err
    );

  if (!err.empty()) {
    return "";
  }

  return extract_audio_url(words);
}

/*
 * Function: extract_pronunciation
 * Description: Extracts pronunciation.
 */
std::string extract_pronunciation(
  const Json words
)
{
  const Json::array &word_arr =
    words.array_items();

  for (const Json &word : word_arr) {
    std::string pronunciation =
      word["phonetic"]
      .string_value();

    if (!pronunciation.empty()) {
      return pronunciation;
    }

    const Json phonetics =
      word["phonetics"];

    const Json::array &
      phonetic_arr =
      phonetics.array_items();

    for (
      const Json &phonetic :
      phonetic_arr
    ) {
      pronunciation =
        phonetic["text"]
        .string_value();

      if (!pronunciation.empty()) {
        return pronunciation;
      }
    }
  }

  return "";
}

/*
 * Function: query_pronunciation
 * Description: Queries pronunciation.
 */
std::string query_pronunciation(
  std::string word
)
{
  httplib::Client cli(
    DICTIONARY_API_HOST
  );

  auto res =
    cli.Get(
      build_request_endpoint(word)
    );

  if (res->status != 200) {
    return "";
  }

  std::string err;

  Json words =
    Json::parse(
      res->body,
      err
    );

  if (!err.empty()) {
    return "";
  }

  return extract_pronunciation(
    words
  );
}

/*
 * Function: displayMenu
 * Description: Displays menu options.
 */
void displayMenu()
{
  std::cout
    << "\n===== Dictionary Menu =====\n";

  std::cout
    << "definition - Look up definition\n";

  std::cout
    << "audio - Look up audio URL\n";

  std::cout
    << "pronunciation - Look up pronunciation\n";

  std::cout
    << "exit - Quit program\n";

  std::cout
    << "Choice: ";
}

/*
 * Function: promptForWord
 * Description: Prompts user for word.
 */
std::string promptForWord()
{
  std::cout
    << "Enter word: ";

  std::string word;

  std::cin >> word;

  return word;
}

/*
 * Function: printLookupResult
 * Description: Prints lookup result.
 */
bool printLookupResult(
  const std::string &label,
  const std::string &information
)
{
  if (information.empty()) {
    std::cout
      << "Information could not "
      << "be found.\n";

    return false;
  }

  std::cout
    << label
    << ": "
    << information
    << '\n';

  return true;
}

/*
 * Function: handleDefinitionLookup
 * Description: Retrieves definition.
 */
void handleDefinitionLookup()
{
  std::string word =
    promptForWord();

  std::string definition =
    query_definition(word);

  printLookupResult(
    "Definition",
    definition
  );
}

/*
 * Function: handleAudioLookup
 * Description: Retrieves audio URL.
 */
void handleAudioLookup()
{
  std::string word =
    promptForWord();

  std::string audioUrl =
    query_audio_url(word);

  printLookupResult(
    "Audio URL",
    audioUrl
  );
}

/*
 * Function: handlePronunciationLookup
 * Description: Retrieves pronunciation.
 */
void handlePronunciationLookup()
{
  std::string word =
    promptForWord();

  std::string pronunciation =
    query_pronunciation(word);

  printLookupResult(
    "Pronunciation",
    pronunciation
  );
}

/*
 * Function: runDictionaryMenu
 * Description: Runs menu loop.
 */
void runDictionaryMenu()
{
  bool keepRunning = true;

  while (keepRunning) {
    displayMenu();

    std::string choice;

    std::cin >> choice;

    if (choice == "definition") {
      handleDefinitionLookup();

    } else if (
      choice == "audio"
    ) {
      handleAudioLookup();

    } else if (
      choice ==
      "pronunciation"
    ) {
      handlePronunciationLookup();

    } else if (
      choice == "exit"
    ) {
      keepRunning = false;

    } else {
      std::cout
        << "Invalid option. "
        << "Please try again.\n";
    }
  }
}

int main()
{
  runDictionaryMenu();

  return 0;
}