#include "phenotype.h"

//------------------------------------Sigmoid function------------------------
//
//----------------------------------------------------------------------------

float Sigmoid(float netinput, float response)
{
	return ( 1 / ( 1 + exp(-netinput / response)));
}


//--------------------------------- ctor ---------------------------------
//
//------------------------------------------------------------------------
CNeuralNet::CNeuralNet(vector<SNeuron*> neurons,
                       int              depth):m_vecpNeurons(neurons),
                                               m_iDepth(depth)
{}


//--------------------------------- dtor ---------------------------------
//
//------------------------------------------------------------------------
CNeuralNet::~CNeuralNet()
{
  //delete any live neurons
  for (int i=0; i<m_vecpNeurons.size(); ++i)
  {
    if (m_vecpNeurons[i])
    {
      delete m_vecpNeurons[i];

      m_vecpNeurons[i] = NULL;
    }
  }
}

//----------------------------------Update--------------------------------
//	takes a list of doubles as inputs into the network then steps through 
//  the neurons calculating each neurons next output.
//
//	finally returns a std::vector of doubles as the output from the net.
//------------------------------------------------------------------------
vector<double> CNeuralNet::Update(const vector<double> &inputs,
                                  const run_type        type)
{
  //create a vector to put the outputs into
  vector<double>	outputs;

  //if the mode is snapshot then we require all the neurons to be
  //iterated through as many times as the network is deep. If the 
  //mode is set to active the method can return an output after
  //just one iteration
  int FlushCount = 0;
  
  if (type == snapshot)
  {
    FlushCount = m_iDepth;
  }
  else
  {
    FlushCount = 1;
  }

  //iterate through the network FlushCount times
  for (int i=0; i<FlushCount; ++i)
  {
    //clear the output vector
    outputs.clear();
   
    //this is an index into the current neuron
    int cNeuron = 0;

    //first set the outputs of the 'input' neurons to be equal
    //to the values passed into the function in inputs
    while (m_vecpNeurons[cNeuron]->NeuronType == input)
    {
      m_vecpNeurons[cNeuron]->dOutput = inputs[cNeuron];

      ++cNeuron;
    }

    //set the output of the bias to 1
    m_vecpNeurons[cNeuron++]->dOutput = 1;

    //then we step through the network a neuron at a time
    while (cNeuron < m_vecpNeurons.size())
    {
      //this will hold the sum of all the inputs x weights 
      double sum = 0;

      //sum this neuron's inputs by iterating through all the links into
      //the neuron
      for (int lnk=0; lnk<m_vecpNeurons[cNeuron]->vecLinksIn.size(); ++lnk)
      {
        //get this link's weight
        double Weight = m_vecpNeurons[cNeuron]->vecLinksIn[lnk].dWeight;

        //get the output from the neuron this link is coming from
        double NeuronOutput =
        m_vecpNeurons[cNeuron]->vecLinksIn[lnk].pIn->dOutput;

        //add to sum
        sum += Weight * NeuronOutput;
      }

      //now put the sum through the activation function and assign the 
      //value to this neuron's output
      m_vecpNeurons[cNeuron]->dOutput = 
      Sigmoid(sum, m_vecpNeurons[cNeuron]->dActivationResponse);

      if (m_vecpNeurons[cNeuron]->NeuronType == output)
      {
        //add to our outputs
        outputs.push_back(m_vecpNeurons[cNeuron]->dOutput);
      }

      //next neuron
      ++cNeuron;
    }

  }//next iteration through the network

#if 1

  //the network needs to be flushed if this type of update is performed otherwise
  //it is possible for dependencies to be built on the order the training data is
  //presented
  if (type == snapshot)
  {
    for (int n=0; n<m_vecpNeurons.size(); ++n)
    {
      m_vecpNeurons[n]->dOutput = 0;
    }
  }

#endif

  //return the outputs
  return outputs;
}


//----------------------------- TidyXSplits -----------------------------
//
//  This is a fix to prevent neurons overlapping when they are displayed
//-----------------------------------------------------------------------
void TidyXSplits(vector<SNeuron*> &neurons)
{
  //stores the index of any neurons with identical splitY values 
  vector<int>    SameLevelNeurons;

  //stores all the splitY values already checked
  vector<double> DepthsChecked;


  //for each neuron find all neurons of identical ySplit level
  for (int n=0; n<neurons.size(); ++n)
  {
    double ThisDepth = neurons[n]->dSplitY;

    //check to see if we have already adjusted the neurons at this depth
    bool bAlreadyChecked = false;

    for (int i=0; i<DepthsChecked.size(); ++i)
    {
      if (DepthsChecked[i] == ThisDepth)
      {
        bAlreadyChecked = true;

        break;
      }
    }

    //add this depth to the depths checked.
    DepthsChecked.push_back(ThisDepth);

    //if this depth has not already been adjusted
    if (!bAlreadyChecked)
    {
      //clear this storage and add the neuron's index we are checking
      SameLevelNeurons.clear();
      SameLevelNeurons.push_back(n);
      
      //find all the neurons with this splitY depth
      for (int i=n+1; i<neurons.size(); ++i)
      {
        if (neurons[i]->dSplitY == ThisDepth)
        {
          //add the index to this neuron
          SameLevelNeurons.push_back(i);
        }
      }

      //calculate the distance between each neuron
      double slice = 1.0/(SameLevelNeurons.size()+1);
  

      //separate all neurons at this level
      for (int i=0; i<SameLevelNeurons.size(); ++i)
      {
        int idx = SameLevelNeurons[i];

        neurons[idx]->dSplitX = (i+1) * slice;
      }
    }

  }//next neuron to check

}
//----------------------------- DrawNet ----------------------------------
//
//  creates a representation of the ANN on a device context
//
//------------------------------------------------------------------------
void CNeuralNet::DrawNet(sf::RenderWindow &window, int Left, int Right, int Top, int Bottom)
{
  //the border width
  const int border = 10;
    
  //max line thickness
  const int MaxThickness = 5;

  TidyXSplits(m_vecpNeurons);

  //go through the neurons and assign x/y coords
  int spanX = Right - Left;
  int spanY = Top - Bottom - (2*border);

  for (int cNeuron=0; cNeuron<m_vecpNeurons.size(); ++cNeuron)
  {
    m_vecpNeurons[cNeuron]->iPosX = Left + spanX*m_vecpNeurons[cNeuron]->dSplitX;
    m_vecpNeurons[cNeuron]->iPosY = (Top - border) - (spanY * m_vecpNeurons[cNeuron]->dSplitY);
  }

  //radius of neurons
  int radNeuron = spanX/60;
  int radLink = radNeuron * 1.5;
  sf::Color drawingColour = sf::Color::White;
  //now we have an X,Y pos for every neuron we can get on with the
  //drawing. First step through each neuron in the network and draw
  //the links
  for (int cNeuron=0; cNeuron<m_vecpNeurons.size(); ++cNeuron)
  {
    //grab this neurons position as the start position of each
    //connection
    int StartX = m_vecpNeurons[cNeuron]->iPosX;
    int StartY = m_vecpNeurons[cNeuron]->iPosY;

    //is this a bias neuron? If so, draw the link in green
    bool bBias = false;

    if (m_vecpNeurons[cNeuron]->NeuronType == bias)
    {
		drawingColour = sf::Color::Green;
      bBias = true;
    }

    //now iterate through each outgoing link to grab the end points
    for (int cLnk=0; cLnk<m_vecpNeurons[cNeuron]->vecLinksOut.size(); ++ cLnk)
    {
      int EndX = m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].pOut->iPosX;
      int EndY = m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].pOut->iPosY;

      //if link is forward draw a straight line
      if( (!m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].bRecurrent) && !bBias)
      {
        int thickness = (int)(fabs(m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].dWeight));        
        
        Clamp(thickness, 0, MaxThickness);

    

        //create a yellow pen for inhibitory weights
        if (m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].dWeight <= 0)
        {
			drawingColour = sf::Color::Yellow;
		}

        //grey for excitory
        else
        {
			drawingColour = sf::Color::White;
        }
        
       // HPEN tempPen = (HPEN)SelectObject(surface, Pen);
        

		//sf::Vertex line[2];
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(StartX, StartY), drawingColour),
			sf::Vertex(sf::Vector2f(EndX, EndY), drawingColour)
		};
        //draw the link

		window.draw(line, 2, sf::Lines);
      
      }

      else if( (!m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].bRecurrent) && bBias)
      {
		  sf::Vertex line[] =
		  {
			  sf::Vertex(sf::Vector2f(StartX, StartY), drawingColour),
			  sf::Vertex(sf::Vector2f(EndX, EndY), drawingColour)
		  };
		  //draw the link

		  window.draw(line, 2, sf::Lines);
      }

      //recurrent link draw in red
      else
      {
        if ((StartX == EndX) && (StartY == EndY))
        {

          //blue for inhibitory
          if (m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].dWeight <= 0)
          {
           drawingColour = sf::Color::Blue;
          }

          //red for excitory
          else
          {
			  drawingColour = sf::Color::Red;
          }

          
          //we have a recursive link to the same neuron draw an ellipse
          int x = m_vecpNeurons[cNeuron]->iPosX ; 
          int y = m_vecpNeurons[cNeuron]->iPosY - (1.5 * radNeuron);

		  sf::CircleShape circle(radLink);
		  circle.setPosition(x - radLink, y + radLink);
		  circle.setOutlineColor(drawingColour);
		  circle.setOutlineThickness(2);
		  circle.setFillColor(sf::Color::Transparent);
		  window.draw(circle);

        }

        else
        {
          int thickness = (int)(fabs(m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].dWeight));

          Clamp(thickness, 0, MaxThickness);

         // HPEN Pen;

          //blue for inhibitory
          if (m_vecpNeurons[cNeuron]->vecLinksOut[cLnk].dWeight <= 0)
          {
			  drawingColour = sf::Color::Blue;
          }

          //red for excitory
          else
          {
			  drawingColour = sf::Color::Red;
          }
          
        
		  sf::Vertex line[] =
		  {
			  sf::Vertex(sf::Vector2f(StartX, StartY), drawingColour),
			  sf::Vertex(sf::Vector2f(EndX, EndY), drawingColour)
		  };
		  //draw the link

		  window.draw(line, 2, sf::Lines);
        }
      }

    }
  }

  //now draw the neurons and their IDs
//  SelectObject(surface, RedBrush);
 // SelectObject(surface, GetStockObject(BLACK_PEN));

  for (int cNeuron=0; cNeuron<m_vecpNeurons.size(); ++cNeuron)
  {
    int x = m_vecpNeurons[cNeuron]->iPosX; 
    int y = m_vecpNeurons[cNeuron]->iPosY;

	sf::CircleShape circle(radNeuron/2.0f);
	circle.setPosition(x- radNeuron / 2.0f, y - radNeuron / 2.0f);
	circle.setOutlineColor(sf::Color::Black);
	circle.setOutlineThickness(2);
	circle.setFillColor(sf::Color::Red);
	window.draw(circle);
  }

 
}




