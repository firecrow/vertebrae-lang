bool regex_match(char *pattern, struct string *source){
    regex_t regex_object;
    int result;
    char buffer[100];

    result = regcomp(&regex_object, pattern, 0);
    if (result) {
        fprintf(stderr, "Could not compile regex, aborting\n");
        exit(1);
    }

    result = regexec(&regex_object, source->content, 0, NULL, 0);
    if (!result) {
        return 1;
    }
    else if (result == REG_NOMATCH) {
        return 0;
    }
    else {
        regerror(result, &regex_object, buffer, sizeof(buffer));
        fprintf(stderr, "Regex match failed: %s, aborting\n", buffer);
        exit(1);
    }

    regfree(&regex_object);
}
