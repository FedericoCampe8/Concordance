/**************************************
 * Node:                              *
 * this class represents a node in    *
 * Prefix tree.                       *
 * Each node holds a content and it   *
 * can be final or not.               *
 * If final, the content can be       *
 * printed and represents a word as   *
 * read from the input file.          *
 * Otherwise it is an internal node.  *
 * Nodes can be split in children     *
 * during insertion of new tokens.    *
 **************************************/
package concordance;

import java.util.ArrayList;
import java.util.LinkedList;

class Node {
  /* Level of the node in the tree */
  protected int nodeLevel;
  /* Id of the node */
  protected static int glbNodeId;
  int nodeId;
  /* List children nodes */
  LinkedList<Node> children;
  /* Final/non final node */
  protected boolean isFinal;
  /* Content of this node */
  Content content;
  
  public Node() {
      isFinal = false;
      this.content = null;
      nodeLevel = 0;
      nodeId = glbNodeId++; 
      children = new LinkedList<Node>();
  }//Node
  
  public Node( Content content ) {
      isFinal = true;
      this.content = content;
      nodeLevel = 0;
      nodeId = glbNodeId++; 
      children = new LinkedList<Node>();
  }//Node
  
  /* Check if a given content matches this.content */
  private boolean matchRoot( Content content ) {
    return (this.content).match( content );
  }//matchRoot
  
  /* Given a content, find the index of the preeceding root */
  private boolean lessRoot ( Content content ) {
    return this.content.less ( content );
  }//lessRoot
  
  /* Given a content,
   * find the index of the preeceding root */
  private int findLowerRoot( Content content ) {
    int found = 0;
    boolean bounds = false;
    for ( ; found < children.size(); found++ ) {
      if ( children.get(found).lessRoot( content ) ) continue;
      bounds = true;
      break;
    }//i
    if ( !bounds ) found = children.size();
    return found;
  }//findLowerRoot
  
  /* Insert a child in the list of children.
   * It preserves the ordering.*/
  private void insertChild ( Node nodeToAdd ) {
    //System.err.println("Arrive child: " + ((Token)nodeToAdd.getContent()).getString() );
    if ( children.size() == 0 ) {
      //System.err.println("Add child: " + ((Token)nodeToAdd.getContent()).getString() );
      children.add( nodeToAdd );
    }
    else {
      int position = findLowerRoot( nodeToAdd.getContent() );
      //System.err.println("Add child in: " + position );
      children.add( position, nodeToAdd );
    }
  }//insertChild
  
  /* Replace children list (used to add/remove nodes in the tree) */
  protected void replaceChildren ( LinkedList<Node> children ) {
    this.children.clear();
    this.children = new LinkedList<Node>(children);
  }//replace_children
  
  /* Fix the current node by adding children or splitting it.
   * Cases:
   * a - the root is equal to the current content:
   *     update root;
   * b - content can match and extend _content:
   *     add a child with that extension (final node);
   * c - content can match and it is a prefix of _content:
   *     split the current node in the match and make it final;
   * d - content partially mathces _content:
   *     split the node until the match and add new nodes.
   */
  protected void fixNode ( Content newContent ) {
    //System.err.println("Fix for: " + ((String)newContent.getRefBaseObj()) );
    /* It returns a vector of contents:
     * the vector is empty         -> complete match           
     *                             -> perform update current content
     * the vector as one entry     -> complete match + extension 
     *                             -> add child
     * the vector as two entries   -> sub match                  
     *                             -> split the previous content
     * the vector as three entries -> partial match              
     *                             -> split and add
     */
    ArrayList< Content > newContents = new ArrayList<Content>();
    //System.err.println("MATCH " + (String)content.getRefBaseObj() );
    this.content.howMatch( newContent, newContents );
   
    /* Check cases */
    if ( newContents.isEmpty() ) {
      //System.err.println("Case 0 " + ((String)newContent.getRefBaseObj()) );
      /* Update the current node (full match).
       * It updates lines and increase counter.
       */
      isFinal = true;
      this.content.updateContent( newContent );
    }
    else if ( newContents.size() == 1 ) {
      //System.err.println("Case 1 " + ((String)newContent.getRefBaseObj()) );
      /* "content" has the form "xy" where "x" is a prefix matching
       * the current content (_content) while "y" is a suffix.
       * Use new_contents[ 0 ] (i.e., the suffix "y") to recursively
       * go down the tree and find the right place for the remaining
       * part "y".
       * @note: there is a subtle thing here. If "y" is equal to "x"
       * then "y" must be added to the list of children of the current
       * node, otherwise "y" will be inserted in the current node with
       * the recursive call of add_content.
       */
       /* Check which child matches new_contents[ 0 ] */
       if (this.content.equal ( newContents.get( 0 ) ) ) {
         boolean foundANode = false;
         for ( int i = 0; i < children.size(); i++ ) {
           if ( children.get(i).matchRoot(newContents.get(0))) {
             /* A match has been found: 
              * proceed recursively with the right child */
             children.get(i).addContent( newContents.get(0));
             foundANode = true;
             break;
           }
         }//for
         if ( !foundANode ) {
           Node newChild = new Node ( newContents.get(0) );
           newChild.setLevel(getLevel() +1 );
           insertChild( newChild );
         }
       }
       else {
         /* No match for "y", proceed recursively */ 
         addContent( newContents.get(0) );
       }
    }
    else if ( newContents.size() == 2 ) {
      //System.err.println("Case 2 " + ((String)newContent.getRefBaseObj()) );
      /* "content" has the form "z" where "z" is a prefix of the current
       * content "x" = "zt". 
       * Split the node and replace the current content with new_contents[ 0 ]
       * (i.e., "z"), make it final and set counter to 1 and add as child 
       * new_contents[ 1 ] (i.e., "t") with all the info of "x":
       * before: zt -> children -> ...
       * after:  z  -> t -> children -> ...
       */
      this.content.updateContent(newContents.get(0));
      /* Create a new node in the tree for new_contents[ 1 ] */
      Node splitChild = new Node( newContents.get(1));
      splitChild.setLevel( getLevel() + 1);
      /* Preserves the _final parameter of the current node */
      splitChild.setFinal( this.isFinal );
      /* Link it to the current children */
      splitChild.replaceChildren( this.children );
      /* Link current node to split_child */
      this.children.clear();
      insertChild( splitChild );
      /* Set the current node as final since "z" is a token */
      this.isFinal = true;
    }
    else {
      /*
       * "content" has the form "xy" where "x" is a prefix
       * of the current content "xt".
       * Split the node and replace the current content with new_contents[ 0 ]
       * (i.e., "x"), make it NOT final and add as child
       * new_contents[ 1 ] (i.e., "t") with all the info of "xt", and 
       * new_contents[ 2 ] (i.e., "y") with lines of "xy" and counter 1.
       * before: xt -> children -> ...
       * after:  x  -> t -> children -> ...
       *            -> y
       */
      
      this.content.updateContent( newContents.get(0) );

      /* Create a new node (i.e., t) */
      Node splitChild = new Node ( newContents.get(1) );
      splitChild.setLevel(getLevel() + 1 );
      increaseChildrenLevel( 1 );
      /* Preserves the _final parameter of the current node */
      splitChild.setFinal( this.isFinal );
      /* Link to the new node the current children */
      splitChild.replaceChildren( this.children );
      this.children.clear();
      /* Link the new node to the current node */
      insertChild ( splitChild );

      /* Create a new node (i.e., y) */
      Node newChild = new Node ( newContents.get(2) );
      newChild.setLevel ( getLevel() + 1 );
      /* Link the new node to the current node */
      insertChild ( newChild );

      /* Set the current node as not final (it has been split) */
      this.isFinal = false; 
    }
  }//fixNode
  
  /* Increase (recursively) the level of the children:
   * useful for splitting edges in the tree. */
  protected void increaseChildrenLevel ( int increment ) {
    for ( int i = 0; i < children.size(); i++ ) {
      children.get(i).setLevel( children.get(i).getLevel() + increment );
      children.get(i).increaseChildrenLevel( increment );
    }//i
  }//increaseChildrenLevel
  
  /* Check if a "prefix" of the current content is present
   * in one of the children; i.e., it checks if one of the
   * children can be a root for the current content.
   * It returns x = -1 if the root is not present or x >= 0,
   * where x is the index of the child corresponding 
   * to the root.
   */
  protected int isRootPresent ( Content content ) {
    int index = -1;
    for ( int i = 0; i < children.size(); i++ ) {
      if ( children.get(i).matchRoot( content ) ) {
        index = i;
        break;
      } 
    }//i
    return index;
  }//isRootPresent
  
  /* Add content to the node.
   * given the current node it can
   * perform several actions:
   * 1 - Set the current content;
   * 2 - Add a content to the current one;
   * 3 - Create a new node and add it to the list
   *     of children in the correct position;
   * 4 - Push down the content to one of the children;
   * 5 - Split the current node and add the content down
   *     on one of the children.
   * This cases create the Prefix tree.
   * It returns a Boolean value to inform whether the
   * new content has been added or not.
   */
  public void addContent ( Content content ) {
    /* Base case: current content is empty
     * Case 1: Add here. */
    if ( this.content == null ) {
      //System.err.println("Insert: " + ((Token) content).getString() );
      this.content = content;
      return;
    }
    
    /* The current content is not empty -> check
     * Case 2: check wheter to add a content to the current one */
    if ( this.content.equal ( content ) ) {
      //System.err.println("Equal: " + ((Token) content).getString() );
      content.updateContent( content );
      return;
    }
    
    /* Case 3: new node if no prefix is found in the list of children */
    int position = isRootPresent( content );
    //System.err.println("Position " + position);
    if ( position == -1 ) {
      /* No match: 
       * Insert it as a new child (new node) the right position 
       * in the list of children. */
      Node nodeToAdd = new Node( content );
      nodeToAdd.setLevel( this.getLevel() + 1 );
      insertChild( nodeToAdd );
    }
    else {
      /* There is a match */
      /*System.err.println("current content " + 
              (String)this.content.getRefBaseObj());*/
      (children.get(position)).fixNode( content );
    }
  }//addContent
  
  
  /* Get/Set methods */
  public boolean isFinal() {
    return isFinal;
  }//isFinal
  
  public void setFinal ( boolean fnl ) {
    isFinal = fnl;
  }//set_final
  
  public int getLevel () {
    return nodeLevel;
  }//getLevel
  
  public void setLevel ( int lvl ) {
    nodeLevel = lvl;
  }//set_level
  
  public int getGlbId () {
    return glbNodeId;
  }//get_glb_id
  
  public int getId () {
    return nodeId;
  }//getId
  
  public Content getContent() {
    return this.content;
  }//getContent

  public void print() {
    String strOut = "NODE_" + nodeId + "\n" +
    "Level: " + nodeLevel + "\n" +
    "Final: ";
    if ( isFinal ) strOut +="T\n";
    else           strOut +="F\n";
    strOut += "Number of children: " + children.size() + "\n";
    strOut += "Content:";
    System.out.println( strOut );
    content.print();
    /*
    for ( int i = 0; i < children.size(); i++ ) {
      children.get(i).print();
    }
     * */
  }//print
  
  /* Accept method for Visitor pattern */
  public void accept( Visitor visitor ) {
    visitor.visitNode ( this );
    for ( int i = 0; i < children.size(); i++ ) {
      children.get(i).accept( visitor );
    }//i
  }
  
}//Node
