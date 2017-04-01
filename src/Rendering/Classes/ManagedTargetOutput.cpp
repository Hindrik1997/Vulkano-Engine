//
// Created by hindrik on 30-3-17.
//

#include "ManagedTargetOutput.h"

ManagedTargetOutput::ManagedTargetOutput(RenderTargetOutput&& output) : m_Output(std::move(output)), m_Manager(m_Output)
{
}