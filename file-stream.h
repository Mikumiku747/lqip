/**
 * @name writeToFile
 * @brief accepts an array of images struct and writes it to file in YAML format.
 *
 * @developer:
 *
 * @param uri
 * @param width
 * @param height
 */
void writeToFile(images_t *images);


/**
 * @name constructYaml
 * @brief accepts key and a value and returns a valid Yaml char array
 *
 * @developer:
 *
 * @param uri
 * @param width
 * @param height
 */
char *constructYaml(char *key, char* val);
