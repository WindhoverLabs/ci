    
#ifndef CI_VERSION_H
#define CI_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

/** \brief Major version number for CI application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined in "ci_platform_cfg.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define CI_MAJOR_VERSION    (1)

/** \brief Minor version number for CI application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined in "ci_platform_cfg.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define CI_MINOR_VERSION    (0)

/** \brief Revision version number for CI application
**  
**  \par Description:
**       An application version number consists of four parts:
**       major version number, minor version number, revision
**       number and mission specific revision number. The mission
**       specific revision number is defined in "ci_platform_cfg.h".
**
**  \par Limits:
**       Must be defined as a numeric value that is greater than
**       or equal to zero.
*/
#define CI_REVISION         (0)


#ifdef __cplusplus
}
#endif

#endif /* CI_VERSION_H */

/************************/
/*  End of File Comment */
/************************/
