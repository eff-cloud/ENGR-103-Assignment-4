/*
 * CHANGE:
 * Replaced repeated menu strings with constants.
 * WHY:
 * Avoids repeating text and follows DRY principle.
 */
const std::string DEFINITION_OPTION =
  "definition";

const std::string AUDIO_OPTION =
  "audio";

const std::string PRONUNCIATION_OPTION =
  "pronunciation";

const std::string EXIT_OPTION =
  "exit";

/*
 * CHANGE:
 * Simplified endpoint creation.
 * WHY:
 * String concatenation is shorter than stringstream.
 */
std::string buildRequestEndpoint(
  const std::string &word
)
{
  return DICTIONARY_API_PATH_PREFIX
    + word;
}

/*
 * CHANGE:
 * Combined repeated API request logic.
 * WHY:
 * Definition, audio, and pronunciation
 * previously repeated the same steps.
 */
bool queryWordsJson(
  const std::string &word,
  Json &words
)
{
  httplib::Client cli(
    DICTIONARY_API_HOST
  );

  auto response =
    cli.Get(
      buildRequestEndpoint(word)
    );

  /*
   * CHANGE:
   * Added response validity check.
   * WHY:
   * Prevents failures if request fails.
   */
  if (
    !response ||
    response->status != 200
  ) {
    return false;
  }

  std::string errorMessage;

  words =
    Json::parse(
      response->body,
      errorMessage
    );

  return errorMessage.empty();
}

/*
 * CHANGE:
 * Definition now reuses queryWordsJson().
 * WHY:
 * Removes repeated API code.
 */
std::string queryDefinition(
  const std::string &word
)
{
  Json words;

  if (
    !queryWordsJson(
      word,
      words
    )
  ) {
    return "";
  }

  return extract_definition(
    words
  );
}

/*
 * CHANGE:
 * Audio lookup reuses queryWordsJson().
 * WHY:
 * Removes duplicate request code.
 */
std::string queryAudioUrl(
  const std::string &word
)
{
  Json words;

  if (
    !queryWordsJson(
      word,
      words
    )
  ) {
    return "";
  }

  return extract_audio_url(
    words
  );
}

/*
 * CHANGE:
 * Pronunciation lookup reuses queryWordsJson().
 * WHY:
 * Reduces repeated logic.
 */
std::string queryPronunciation(
  const std::string &word
)
{
  Json words;

  if (
    !queryWordsJson(
      word,
      words
    )
  ) {
    return "";
  }

  return extract_pronunciation(
    words
  );
}

void displayMenu()
{
  std::cout
    << "\n===== Dictionary Menu =====\n";

  std::cout
    << DEFINITION_OPTION
    << " - Look up definition\n";

  std::cout
    << AUDIO_OPTION
    << " - Look up audio URL\n";

  std::cout
    << PRONUNCIATION_OPTION
    << " - Look up pronunciation\n";

  std::cout
    << EXIT_OPTION
    << " - Quit program\n";
}

/*
 * CHANGE:
 * Separated menu input into function.
 * WHY:
 * Better responsibility separation.
 */
std::string promptMenuChoice()
{
  std::cout
    << "Choice: ";

  std::string choice;

  std::cin >> choice;

  return choice;
}

std::string promptForWord()
{
  std::cout
    << "Enter word: ";

  std::string word;

  std::cin >> word;

  return word;
}

void printLookupResult(
  const std::string &label,
  const std::string &information
)
{
  if (information.empty()) {
    std::cout
      << "Information could "
      << "not be found.\n";

    return;
  }

  std::cout
    << label
    << ": "
    << information
    << '\n';
}

/*
 * CHANGE:
 * Combined three lookup handlers into one.
 * WHY:
 * Avoids repeating nearly identical code.
 */
void performLookup(
  const std::string &label,
  std::string (
    *queryFunction
  )(const std::string &)
)
{
  std::string word =
    promptForWord();

  std::string result =
    queryFunction(word);

  printLookupResult(
    label,
    result
  );
}

/*
 * CHANGE:
 * Moved invalid option handling into
 * separate function.
 * WHY:
 * Keeps menu logic cleaner.
 */
void handleInvalidChoice()
{
  std::cout
    << "Invalid option. "
    << "Please try again.\n";
}

/*
 * CHANGE:
 * Moved menu processing into function.
 * WHY:
 * Keeps runDictionaryMenu shorter.
 */
void processMenuChoice(
  const std::string &choice,
  bool &keepRunning
)
{
  if (
    choice ==
    DEFINITION_OPTION
  ) {
    performLookup(
      "Definition",
      queryDefinition
    );

  } else if (
    choice ==
    AUDIO_OPTION
  ) {
    performLookup(
      "Audio URL",
      queryAudioUrl
    );

  } else if (
    choice ==
    PRONUNCIATION_OPTION
  ) {
    performLookup(
      "Pronunciation",
      queryPronunciation
    );

  } else if (
    choice ==
    EXIT_OPTION
  ) {
    keepRunning = false;

  } else {
    handleInvalidChoice();
  }
}

void runDictionaryMenu()
{
  bool keepRunning =
    true;

  while (keepRunning) {
    displayMenu();

    std::string choice =
      promptMenuChoice();

    processMenuChoice(
      choice,
      keepRunning
    );
  }
}

int main()
{
  runDictionaryMenu();

  return 0;
}