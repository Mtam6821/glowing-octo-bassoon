/**************************************************
Scene.cpp contains the implementation of the draw command
*****************************************************/
#include "RTScene.h"
#include "RTCube.h"
#include "RTObj.h"

// The scene init definition 
//#include "RTScene.inl"
//#include "RTSceneTest.inl"
#include "OriginalRTScene.inl"

#define GLM_SWIZZLE

using namespace glm;
void RTScene::buildTriangleSoup(void){
    // Pre-draw sequence: assign uniforms that are the same for all Geometry::draw call.  These uniforms include the camera view, proj, and the lights.  These uniform do not include modelview and material parameters.
    camera -> computeMatrices();
    shader -> view = camera -> view;
    shader -> projection = camera -> proj;
    shader -> nlights = light.size();
    shader -> lightpositions.resize( shader -> nlights );
    shader -> lightcolors.resize( shader -> nlights );
    int count = 0;
    for (std::pair<std::string, Light*> entry : light){
        shader -> lightpositions[ count ] = (entry.second) -> position;
        shader -> lightcolors[ count ] = (entry.second) -> color;
        count++;
    }
    
    // Define stacks for depth-first search (DFS)
    std::stack < RTNode* > dfs_stack;
    std::stack < mat4 >  matrix_stack; // HW3: You will update this matrix_stack during the depth-first search while loop.
    
    // Initialize the current state variable for DFS
    RTNode* cur = node["world"]; // root of the tree
    mat4 cur_VM = camera -> view; // HW3: You will update this current modelview during the depth first search.  Initially, we are at the "world" node, whose modelview matrix is just camera's view matrix.
    
    // HW3: The following is the beginning of the depth-first search algorithm.
    // HW3: The depth-first search for the node traversal has already been implemented (cur, dfs_stack).
    // HW3: All you have to do is to also update the states of (cur_VM, matrix_stack) alongside the traversal.  You will only need to modify starting from this line.
    dfs_stack.push(cur);
    matrix_stack.push(cur_VM);
    
    // Compute total number of connectivities in the graph; this would be an upper bound for
    // the stack size in the depth first search over the directed acyclic graph
    int total_number_of_edges = 0; 
    for ( const auto &n : node ) total_number_of_edges += n.second->childnodes.size();
    
    // If you want to print some statistics of your scene graph
     std::cout << "total numb of nodes = " << node.size() << std::endl;
     std::cout << "total number of edges = " << total_number_of_edges << std::endl;
    
    while( ! dfs_stack.empty() ){
        // Detect whether the search runs into infinite loop by checking whether the stack is longer than the number of edges in the graph.
        if ( dfs_stack.size() > total_number_of_edges ){
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }

        //stack sizes are always equal
        assert(dfs_stack.size() == matrix_stack.size());

        // top-pop the stacks
        cur = dfs_stack.top();  dfs_stack.pop();
        cur_VM = matrix_stack.top();    matrix_stack.pop();
        
        // draw all the models at the current node
        for ( size_t i = 0; i < cur -> models.size(); i++ ){
            // Prepare to draw the geometry. Assign the modelview and the material.
            
            //get the vm to use with this model by getting the required matrix and multiplying
            mat4 model_VM = cur_VM * cur->modeltransforms[i];

            shader -> modelview = model_VM; 
            shader -> material  = ( cur -> models[i] ) -> material;
            shader -> setUniforms();


            auto elements = (cur->models[i])->geometry->elements;
            for (int ind = 0; ind < elements.size(); ind++) {
                Triangle currentTri = elements[ind];

                currentTri.material = (cur->models[i])->material;        //set mat
                for (int j = 0; j < currentTri.P.size(); j++) {

                    glm::vec4 p = glm::vec4(currentTri.P[j], 1.0f);
                    p = model_VM * p;
                    currentTri.P[j] = glm::vec3(p) / p.w;

                    currentTri.N[j] = glm::normalize(glm::transpose(glm::inverse(glm::mat3(model_VM))) * currentTri.N[j]);
                }
               
                triangle_soup.push_back(currentTri);
            }
        }
        
        // Continue the DFS: put all the child nodes of the current node in the stack
        for ( size_t i = 0; i < cur -> childnodes.size(); i++ ){
            dfs_stack.push( cur -> childnodes[i] );
            matrix_stack.push( cur_VM * cur->childtransforms[i] );
        }
        
    } // End of DFS while loop.
    // HW3: Your code will only be above this line.
    std::cout << "triangle soup built" << std::endl;
}



