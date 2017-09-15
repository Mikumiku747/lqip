/**
 * @file: yaml.h
 * @subject: 48430 Embedded C - Assignment 3
 * @developers:
 *   @name: Michael Novak  @student_id: 12887648
 *   @name:                @student_id:
 *   @name:                @student_id:
 *   @name:                @student_id:
 *
 * @date: 08-09-2017
 *
 * @achieved:
 *
 * @pending:
 *
 * @note:
 *
 * @TODO:
 *
 */

/* Include Guard */
#ifndef YAML_H
#define YAML_H

 
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


#endif
