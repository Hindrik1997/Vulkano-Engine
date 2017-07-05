//
// Created by hindrik on 30-6-2017.
//

#ifndef VULKANOENGINE_MEMBLOCKALLOCATOR_H
#define VULKANOENGINE_MEMBLOCKALLOCATOR_H

#include <cstdint>
#include <utility>
#include <array>

using std::pair;
using std::array;


template<typename block_size_type, uint16_t subdiv_count, block_size_type max_size>
constexpr uint16_t sub_div_calc()
{
    static_assert(subdiv_count != 0);
    static_assert(subdiv_count < (UINT16_MAX - 1));
    static_assert(max_size != 0 && (max_size & (max_size - 1)) == 0);

    block_size_type cSize = max_size;
    uint16_t cSubDivC = subdiv_count;

    uint16_t ret = 0;
    while(cSubDivC != 0)
    {
        ret += cSubDivC;
        cSize /= 2;
        if(cSize == 1 && cSubDivC > 1)
            throw;
        cSubDivC--;
    }
    return ret;
};



template <typename block_size_type, block_size_type max_size, uint16_t subdiv_count>
class MemBlockAllocator final
{
    array<block_size_type, sub_div_calc<block_size_type, subdiv_count, max_size>()> m_SubDivisions;








    pair<bool, block_size_type> getMemory(block_size_type size);
    void returnMemory(block_size_type offset);



};


template<typename block_size_type, block_size_type max_size, uint16_t subdiv_count>
pair<bool, block_size_type> MemBlockAllocator<block_size_type, max_size, subdiv_count>::getMemory(block_size_type size)
{
    return pair<bool, block_size_type>();
}

template<typename block_size_type,block_size_type max_size, uint16_t subdiv_count>
void MemBlockAllocator<block_size_type, max_size, subdiv_count>::returnMemory(block_size_type offset)
{

}


#endif //VULKANOENGINE_MEMBLOCKALLOCATOR_H