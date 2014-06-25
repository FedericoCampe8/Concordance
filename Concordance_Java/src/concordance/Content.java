/*******************************************
 * Content:                                *
 * this class represents the               *
 * interface of the objects                *
 * stored in each node of the Prefix tree. *
 * Concrete implementations should         *
 * implement this interface.               *
 *******************************************/
package concordance;

import java.util.ArrayList;

interface Content {
  /* Returns an object that corresponds to the base object of the content:
   * this is useful to extract such object if needed for analyzing it. */
  Object getRefBaseObj ();
  
  /* Methods used for comparing contents in the data structure.
   * These methods are used for sorting the contents. */
  boolean equal( Content content );
  boolean less ( Content content );
 
  /* Check if a content matches the current one */
  boolean match(Content content);

  /* Return the size of the base object
   * representing the content.
   * This is useful to organize or print contents. */
  int getSize ();
  
  /* Update the current content with a new content 
   * given in input:
   * different derived class may change 
   * how to perform the update. */
  void updateContent(Content content);

  /* Check "how" a given content matches the current one.
   * It returns a vector of contents:
   * 1 - the vector is empty -> complete match 
   *                         -> perform update current content;
   * 2 - the vector as one entry -> complete match + extension 
   *                             -> add child;
   * 3 - the vector as three entries -> partial match 
   *                                 -> split and add.
   * @note: it does not modify the current content if split.
   * the client has to over-write it with the content given back!
   * @note: different implementations may define matches in
   * different ways.
   */
  void howMatch(Content content, ArrayList<Content> matches );
  
  /* Print the content on stdout */
  void print();
  /* Print info (general).
   * The client can customize this method if needed.
   * @note: the above "print" prints all the info related to 
   * the content, while "print_info" can be customized by the client
   * to print only some information related to the content.
   */
   void   printInfo ();
   String getInfo   ();
}//Content
