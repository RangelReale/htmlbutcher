//=============================================================================
/**
 *  @file   ButcherProjectConsts.h
 *
 *  $Id$
 *
 *  @author Rangel Reale <rreale@bol.com.br>
 *  @date   2008-02-05
 */
//=============================================================================
#ifndef __BPROJECT_BUTCHERPROJECTCONSTS_H__
#define __BPROJECT_BUTCHERPROJECTCONSTS_H__

/**
 * @class ButcherProjectConsts
 *
 * @brief Project consts
 */
class ButcherProjectConsts
{
public:
	enum layouttype_t { LTYPE_DEFAULT = 0, LTYPE_TABLE = 1, LTYPE_APTABLELESS = 2 };
	static const layouttype_t DEFAULT_LAYOUTTYPE = LTYPE_TABLE;
};


#endif // __BPROJECT_BUTCHERPROJECTCONSTS_H__